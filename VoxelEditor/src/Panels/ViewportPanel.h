#pragma once

#include "Voxel/Core/Core.h"
#include "Voxel/Rendering/Framebuffer.h"

#include "Panel.h"
#include "../EditorState.h"

#include <imgui.h>

#include <glm/glm.hpp>

namespace Voxel {

	class EditorLayer;

	class ViewportPanel : public Panel 
	{
	public:
		ViewportPanel(EditorState& state)	
			: m_State(state), Panel("Viewport") 
		{
			FramebufferSpecification viewportSpec;
			viewportSpec.Width = 1280;
			viewportSpec.Height = 720;
			viewportSpec.Attachments = {
				FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
				FramebufferTextureFormat::Depth
			};

			m_Framebuffer = Framebuffer::Create(viewportSpec);
		}

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		bool OnWindowRestored(WindowRestoreEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		bool ShouldResize()
		{
			FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			return m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
				(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y);
		}

	private:
		bool m_Focused = false;
		bool m_Hovered = false;
		bool m_Active = false;

		EditorState& m_State;
		int m_GizmoType = -1;

		EditorCamera m_Camera;
		Ref<Framebuffer> m_Framebuffer;

		ImVec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2] = { glm::vec2(0.0f) };

		friend class EditorLayer;
	};

}