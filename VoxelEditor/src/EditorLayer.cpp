#include "EditorLayer.h"

#include "Voxel/Scene/SceneSerializer.h"

#include <ImGui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ViewportPanel.h"

namespace Voxel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		m_EditorState.ActiveScene = CreateRef<Scene>();
		m_EditorState.CurrentContext = {};

		auto propertiesPanel = CreateRef<PropertiesPanel>(m_EditorState);
		auto sceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_EditorState);
		auto viewportPanel = CreateRef<ViewportPanel>(m_EditorState);
		auto contentBrowserPanel = CreateRef<ContentBrowserPanel>(m_EditorState);

		m_Panels.push_back(sceneHierarchyPanel);
		m_Panels.push_back(propertiesPanel);
		m_Panels.push_back(viewportPanel);
		m_Panels.push_back(contentBrowserPanel);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep& timestep)
	{
		for (auto& panel : m_Panels)
		{
			panel->OnUpdate(timestep);
		}
	}

	void EditorLayer::OnEvent(Event& event)
	{
		for (const auto& panel : m_Panels)
		{
			panel->OnEvent(event);
		}
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
					else if (control)
						SaveScene();
					break;

				// Gizmos
				case KeyCode::Q:
					m_EditorState.GizmoType = -1;
					break;
				case KeyCode::W:
					m_EditorState.GizmoType = ImGuizmo::OPERATION::TRANSLATE;
					break;
				case KeyCode::E:
					m_EditorState.GizmoType = ImGuizmo::OPERATION::ROTATE;
					break;
				case KeyCode::R:
					m_EditorState.GizmoType = ImGuizmo::OPERATION::SCALE;
					break;
			}
			return true;
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_EditorState.ActiveScene = CreateRef<Scene>();
	}

	void EditorLayer::OpenScene()
	{
		auto path = FileDialogs::OpenFile(VOXEL_SCENE_FILE_FILTER);
		if (!path.empty())
		{
			m_EditorState.ActiveScene = CreateRef<Scene>();
			SceneSerializer serializer(m_EditorState.ActiveScene);
			serializer.DeserializeText(path);
			m_EditorState.CurrentContext = {};
			Application::Get().GetWindow().SetTitle("VoxelEditor - " + m_EditorState.ActiveScene->GetName());
		}
	}
	void EditorLayer::SaveScene()
	{
		if (m_EditorState.ActiveScene->GetSavePath().empty())
		{
			SaveSceneAs();
			return;
		}

		SceneSerializer serializer(m_EditorState.ActiveScene);
		serializer.SerializeText(m_EditorState.ActiveScene->GetSavePath());
		Application::Get().GetWindow().SetTitle("VoxelEditor - " + m_EditorState.ActiveScene->GetName());
	}

	void EditorLayer::SaveSceneAs()
	{
		auto path = FileDialogs::SaveFile(VOXEL_SCENE_FILE_FILTER);
		if (!path.empty())
		{
			auto sceneName = path.stem().string();
			m_EditorState.ActiveScene->SetName(sceneName);
			m_EditorState.ActiveScene->SetSavePath(path);

			SceneSerializer serializer(m_EditorState.ActiveScene);
			serializer.SerializeText(path);
			Application::Get().GetWindow().SetTitle("VoxelEditor - " + m_EditorState.ActiveScene->GetName());
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
		ImGui::Begin("Dockspace", nullptr, window_flags); // Dockspace

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

				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Close"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::BeginMenu("Panels"))
				{
					if (ImGui::MenuItem("Properties"))
						m_Panels.push_back(CreateRef<PropertiesPanel>(m_EditorState));
					if (ImGui::MenuItem("Scene Hierarchy"))
						m_Panels.push_back(CreateRef<SceneHierarchyPanel>(m_EditorState));
					if (ImGui::MenuItem("Viewport"))
						m_Panels.push_back(CreateRef<ViewportPanel>(m_EditorState));
					if (ImGui::MenuItem("Content Browser"))
						m_Panels.push_back(CreateRef<ContentBrowserPanel>(m_EditorState));

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		for (auto& panel : m_Panels)
			panel->OnImGuiRender();

		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		ImGui::End(); // Dockspace
	}

}