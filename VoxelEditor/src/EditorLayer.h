#pragma once

#include "Voxel.h"

#include <ImGui/imgui.h>
#include <imterm/terminal.hpp>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/LogPanel.h"

#include "Voxel/Rendering/EditorCamera.h"

namespace Voxel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(Timestep & timestep);
		virtual void OnEvent(Event & event);

		virtual void OnImGuiRender();
	private:
		bool EditorLayer::OnKeyPressed(KeyPressedEvent& event);
		bool EditorLayer::OnWindowRestored(WindowRestoreEvent& event);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = true;
		bool m_ViewportHovered = false;

		EditorCamera m_EditorCamera;

		Ref<Scene> m_ActiveScene;

		int m_GizmoType = -1;

		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Ref<PropertiesPanel> m_PropertiesPanel;
		//LogPanel m_LogPanel;
	};

}