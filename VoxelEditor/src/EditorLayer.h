#pragma once

#include "Voxel.h"

#include <ImGui/imgui.h>
#include <imterm/terminal.hpp>

#include "Panels/Panel.h"
#include "EditorState.h"

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
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnWindowRestored(WindowRestoreEvent& event);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2] = { glm::vec2(0.0f) };
		bool m_ViewportFocused = true;
		bool m_ViewportHovered = false;

		EditorState m_EditorState;
		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		std::vector<Ref<Panel>> m_Panels;
	};

}