#include "EditorLayer.h"

#include "Voxel/Scene/SceneSerializer.h"

#include <ImGui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Voxel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_PropertiesPanel()
	{
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification viewportSpec;
		viewportSpec.Width = 1280;
		viewportSpec.Height = 720;
		viewportSpec.Attachments = { 
			FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, 
			FramebufferTextureFormat::Depth 
		};

		m_Framebuffer = Framebuffer::Create(viewportSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>();
		m_PropertiesPanel = CreateRef<PropertiesPanel>();
		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
		m_SceneHierarchyPanel->SetPropertiesPanel(m_PropertiesPanel);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep& timestep)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t) m_ViewportSize.x, (uint32_t) m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t) m_ViewportSize.x, (uint32_t) m_ViewportSize.y);
		}

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f ,0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		if (m_ViewportFocused)
		{
			m_EditorCamera.OnUpdate(timestep);
		}
		m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);

		if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
		{

			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			auto [mouseX, mouseY] = ImGui::GetMousePos();
			mouseX -= m_ViewportBounds[0].x;
			mouseY -= viewportSize.y;
			mouseY = m_ViewportBounds[0].y - mouseY; // Invert for OpenGL

			if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				LOG_CORE_DEBUG("Pixel Data: {0}", pixelData);
			}
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<WindowRestoreEvent>(BIND_EVENT_FN(EditorLayer::OnWindowRestored));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() == 0)
		{
			bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
			bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

			switch (event.GetKeyCode())
			{
				case KeyCode::N:
					if (control)
						NewScene();
					break;

				case KeyCode::O:
					if (control)
						OpenScene();
					break;

				case KeyCode::S:
					if (control && shift)
						SaveSceneAs();
					break;

				// Gizmos
				case KeyCode::Q:
					m_GizmoType = -1;
					break;
				case KeyCode::W:
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
					break;
				case KeyCode::E:
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
					break;
				case KeyCode::R:
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
					break;
			}
			return true;
		}
		return false;
	}

	bool EditorLayer::OnWindowRestored(WindowRestoreEvent& event)
	{
		m_ViewportSize = { (float)event.GetWidth(), (float)event.GetHeight() };
		m_Framebuffer->Resize(event.GetWidth(), event.GetHeight());
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenScene()
	{
		auto& path = FileDialogs::OpenFile(VOXEL_SCENE_FILE_FILTER);
		if (!path.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(path);
			m_SceneHierarchyPanel->SetContext(m_ActiveScene);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			Application::Get().GetWindow().SetTitle("VoxelEditor - " + m_ActiveScene->GetName());
		}
	}
	void EditorLayer::SaveScene()
	{

	}
	void EditorLayer::SaveSceneAs()
	{
		auto& path = FileDialogs::SaveFile(VOXEL_SCENE_FILE_FILTER);
		if (!path.empty())
		{
			auto& sceneName = path.stem().string();
			m_ActiveScene->SetName(sceneName);

			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeText(path);
			Application::Get().GetWindow().SetTitle("VoxelEditor - " + m_ActiveScene->GetName());
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, { 0.0f, 0.0f }, dockspace_flags);
		}

		style.WindowMinSize.x = 32.0f;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Close"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel->OnImGuiRender();
		m_PropertiesPanel->OnImGuiRender();

		ImGui::Begin("Debug");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// Viewport

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos();

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		m_ViewportSize = ImGui::GetContentRegionAvail();

		uint32_t textureID = m_Framebuffer->GetColorAttachments()[0];
		ImGui::Image((void*)textureID, m_ViewportSize, { 0, 1 }, { 1, 0 });

		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0].x = minBound.x;
		m_ViewportBounds[0].y = minBound.y;
		m_ViewportBounds[1].x = maxBound.x;
		m_ViewportBounds[1].y = maxBound.y;

		// Gizmos
		Entity& selectedEntity = m_SceneHierarchyPanel->GetPropertiesPanel()->GetContext();
		if (selectedEntity && selectedEntity.HasComponent<TransformComponent>() && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImVec2 pos = ImGui::GetWindowPos();
			ImVec2 size = ImGui::GetWindowSize();
			ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

			const glm::mat4& view = m_EditorCamera.GetViewMatrix();
			const glm::mat4& projection = m_EditorCamera.GetProjection();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4& transform = tc.GetTransform();
			glm::mat4 deltaTransform = transform;

			glm::vec3 previousRotation = tc.LocalRotation;
			glm::vec3 mappedRotation;
			Math::DecomposeRotation(transform, mappedRotation);

			// Snapping
			bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
			float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(
				glm::value_ptr(view), glm::value_ptr(projection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL,
				glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr
			);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position;
				glm::vec3 rotation;
				glm::vec3 scale;
				Math::DecomposeTransform(transform, position, rotation, scale);

				tc.LocalPosition = position;
				tc.LocalRotation = previousRotation - mappedRotation + rotation;
				tc.LocalScale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

}