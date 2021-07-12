#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Voxel {

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(GetImGuiID().c_str());
		
		m_State.ActiveScene->m_Registry.each([=](auto entityID) {
			Entity entity { entityID, m_State.ActiveScene.get() };
			DrawEntityNode(entity);
		});

		//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		//	m_State.CurrentContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_State.ActiveScene->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
	}


	void SceneHierarchyPanel::OnUpdate(Timestep& timestep)
	{

	}

	void SceneHierarchyPanel::OnEvent(Event& event)
	{

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<EntityTagComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_State.CurrentContext == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, tag.Tag.c_str());
		if(ImGui::IsItemClicked())
			m_State.CurrentContext  = entity;

		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				deleteEntity = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			/*for (auto* child : entity.GetChildren())
			{
				DrawEntityNode(*child);
				ImGui::TreePop();
			}*/

			ImGui::TreePop();
		}

		if (deleteEntity)
		{
			m_State.ActiveScene->DestroyEntity(entity);
			if (m_State.CurrentContext == entity)
				m_State.CurrentContext = {};
		}

	}

}