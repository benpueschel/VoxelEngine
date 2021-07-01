#pragma once

#include "Voxel.h"

#include <ImGui/imgui.h>
#include <imterm/terminal.hpp>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/LogPanel.h"

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

		bool m_ViewportFocused = true;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;

		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Ref<PropertiesPanel> m_PropertiesPanel;
		//LogPanel m_LogPanel;
	};

}