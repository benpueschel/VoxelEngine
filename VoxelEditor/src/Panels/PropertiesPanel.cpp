#include "PropertiesPanel.h"

#include <imgui.h>

#include "Voxel/Scene/Components/Components.h"

namespace Voxel {

	void PropertiesPanel::OnImGuiRender()
	{
		if(!m_Locked)
			m_Context = m_State.CurrentContext;

		ImGui::Begin(GetImGuiID().c_str());

		if (m_Context)
		{
			if (ImGui::Button(m_Locked ? "Unlock" : "Lock"))
				m_Locked = !m_Locked;

			const static auto treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_FramePadding
				| ImGuiTreeNodeFlags_AllowItemOverlap;
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			if (m_Context.HasComponent<EntityTagComponent>())
				m_Context.GetComponent<EntityTagComponent>().OnImGuiRender();
			ImGui::PopItemWidth();

			DrawComponent<TransformComponent>("Transform", treeNodeFlags, false);
			DrawComponent<CameraComponent>("Camera", treeNodeFlags);
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", treeNodeFlags);

			ImGui::Separator();

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - UI::GetButtonSize("Add Component").x) * 0.5f);
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_Context.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
				{
					m_Context.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (!m_Context.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
				{
					m_Context.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void PropertiesPanel::OnEvent(Event& event)
	{

	}

	void PropertiesPanel::OnUpdate(Timestep& timestep)
	{

	}

	template<typename Component>
	void PropertiesPanel::DrawComponent(const char* name, int treeNodeFlags, bool removable)
	{

		if (m_Context.HasComponent<Component>())
		{
			ImVec2 contexntRegionAvail = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 4 });
			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::Separator();

			bool opened = ImGui::TreeNodeEx((void*) typeid(Component).hash_code(), treeNodeFlags, name);
			bool removeComponent = false;

			ImGui::PopStyleVar();

			if (removable)
			{
				ImGui::SameLine(contexntRegionAvail.x - lineHeight * 0.5f);
				if (ImGui::Button("...", { lineHeight, lineHeight }))
					ImGui::OpenPopup("ComponentSettings");

				if (ImGui::BeginPopup("ComponentSettings"))
				{
					removeComponent = ImGui::MenuItem("Remove Component");
					ImGui::EndPopup();
				}
			}

			if (opened)
			{
				m_Context.GetComponent<Component>().OnImGuiRender();
				ImGui::TreePop();
			}

			if (removeComponent)
				m_Context.RemoveComponent<Component>();

		}
	}

}