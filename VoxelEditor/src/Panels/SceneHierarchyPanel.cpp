#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Voxel {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		if(m_Properties)
			m_Properties->m_Context = {};
	}

	void SceneHierarchyPanel::SetPropertiesPanel(const Ref<PropertiesPanel>& panel)
	{
		m_Properties = panel;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->m_Registry.each([=](auto entityID) {
			Entity entity { entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		//	m_Properties->m_Context = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_Properties->m_Context == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, tag.Tag.c_str());
		if(ImGui::IsItemClicked())
			m_Properties->SetContext(entity);

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
			m_Context->DestroyEntity(entity);
			if (m_Properties->m_Context == entity)
				m_Properties->m_Context = {};
		}

	}

}