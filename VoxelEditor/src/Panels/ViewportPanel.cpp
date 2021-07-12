#include "ViewportPanel.h"

#include <imgui.h>
#include <cstdint>

#include <ImGuizmo.h>

#include "Voxel/Core/Core.h"
#include "Voxel/Events/Event.h"

namespace Voxel {

	void ViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		m_Active = ImGui::Begin(GetImGuiID().c_str());

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		m_ViewportSize = ImGui::GetContentRegionAvail();

		uint32_t textureID = m_Framebuffer->GetColorAttachments()[0];
		ImGui::Image((void*)textureID, m_ViewportSize, { 0, 1 }, { 1, 0 });

		// Gizmos
		Entity& selectedEntity = m_State.CurrentContext;
		if (selectedEntity && selectedEntity.HasComponent<TransformComponent>() && m_State.GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImVec2 pos = ImGui::GetWindowPos();
			ImVec2 size = ImGui::GetWindowSize();
			ImGuizmo::SetRect(
				m_ViewportBounds[0].x, m_ViewportBounds[0].y,
				m_ViewportBounds[1].x - m_ViewportBounds[0].x,
				m_ViewportBounds[1].y - m_ViewportBounds[0].y
			);
			const glm::mat4& view = m_Camera.GetViewMatrix();
			const glm::mat4& projection = m_Camera.GetProjection();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();
			glm::mat4 deltaTransform = transform;

			glm::vec3 previousRotation = tc.LocalRotation;
			glm::vec3 mappedRotation;
			Math::DecomposeRotation(transform, mappedRotation);

			// Snapping
			bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
			float snapValue = m_State.GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

			float snapValues[3] = { snapValue, snapValue, snapValue };
			ImGuizmo::OPERATION operation = static_cast<ImGuizmo::OPERATION>(m_State.GizmoType);

			ImGuizmo::Manipulate(
				glm::value_ptr(view), glm::value_ptr(projection),
				operation, ImGuizmo::LOCAL,
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
	}

	void ViewportPanel::OnUpdate(Timestep& timestep)
	{
		if (!m_Active) return;

		if (ShouldResize())
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
		m_State.ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f ,0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		if (m_Focused && !ImGuizmo::IsUsing())
			m_Camera.OnUpdate(timestep);

		m_State.ActiveScene->OnUpdateEditor(timestep, m_Camera);

		m_Framebuffer->Unbind();
	}

	void ViewportPanel::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowRestoreEvent>(BIND_EVENT_FN(ViewportPanel::OnWindowRestored));

		if (m_Active)
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ViewportPanel::OnMouseButtonPressed));

		if(m_Active && m_Focused && !ImGuizmo::IsUsing())
			m_Camera.OnEvent(event);
	}

	bool ViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
		bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
		bool alt = Input::IsKeyPressed(KeyCode::LeftAlt) || Input::IsKeyPressed(KeyCode::RightAlt);

		if (alt) return true; // If the User is moving the editor camera, don't mouse pick

		m_Framebuffer->Bind();

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		auto [mouseX, mouseY] = ImGui::GetMousePos();
		mouseX -= m_ViewportBounds[0].x;
		mouseY -= viewportSize.y;
		mouseY = m_ViewportBounds[0].y - mouseY; // Invert for OpenGL

		if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);

			// TODO: If control is pressed, add entity to selection context

			m_State.CurrentContext = pixelData == -1 ? Entity{}
			: Entity{ (entt::entity)pixelData, m_State.ActiveScene.get() };

			event.Handled = true;
			return true;
		}

		m_Framebuffer->Unbind();
		return false;
	}

	bool ViewportPanel::OnWindowRestored(WindowRestoreEvent& event)
	{
		m_ViewportSize = { (float)event.GetWidth(), (float)event.GetHeight() };
		m_Framebuffer->Resize(event.GetWidth(), event.GetHeight());
		return false;
	}

}