#pragma once

#include "Voxel.h"

#include <ImGui/imgui.h>

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
		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize;

		bool m_ViewportFocused = true;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;
		Entity m_Entity;
		Entity m_CameraEntity;
	};

}