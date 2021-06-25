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
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<Texture2D> m_Texture;
		ImVec2 m_ViewportSize;

		bool m_ViewportFocused;
		bool m_ViewportHovered;
	};

}