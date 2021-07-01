#include "EditorLayer.h"

#include "Voxel/Scene/SceneSerializer.h"

#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "EditorCamera.h"

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
			m_ActiveScene->OnViewportResize((uint32_t) m_ViewportSize.x, (uint32_t) m_ViewportSize.y);
		}

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f ,0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(timestep);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused);

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
		//m_LogPanel.OnImGuiRender();

		ImGui::Begin("Debug");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// TODO: Viewport

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		m_ViewportSize = ImGui::GetContentRegionAvail();

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, m_ViewportSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

}