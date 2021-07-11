#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Voxel {

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(GetImGuiID().c_str());
		
		m_State.ActiveScene->m_Registry.each([=](auto entityID) {
			Entity entity{ entityID, m_State.ActiveScene.get() };
			if (!entity.HasComponent<EntityRelationshipComponent>()) return;

			auto& relationship = entity.GetComponent<EntityRelationshipComponent>();
			if(!relationship.Parent)
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

	void SceneHierarchyPanel::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(SceneHierarchyPanel::OnKeyPressed));
	}

	bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
			case KeyCode::Delete:
				if (m_State.CurrentContext)
				{
					m_State.ActiveScene->DestroyEntity(m_State.CurrentContext);
					m_State.CurrentContext = {};
					return true;
				}
				break;
		}

		return false;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& relationship = entity.GetComponent<EntityRelationshipComponent>();
		auto& tag = entity.GetComponent<EntityTagComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_State.CurrentContext == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t) entity, flags, tag.Tag.c_str());
		if(ImGui::IsItemClicked())
			m_State.CurrentContext  = entity;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				m_State.ActiveScene->DestroyEntity(entity);
				if (m_State.CurrentContext == entity)
					m_State.CurrentContext = {};
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			if (m_State.CurrentContext && relationship.FirstChild)
			{
				Entity& currentChild = relationship.FirstChild;
				DrawEntityNode(currentChild);
				for (size_t i = 0; i < relationship.Children - 1; i++)
				{
					currentChild = currentChild.GetOrAddComponent<EntityRelationshipComponent>().Next;
					DrawEntityNode(currentChild);
				}

			}

			ImGui::TreePop();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(GetName(), &entity, sizeof(entity));
			ImGui::Text(tag.Tag.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName());
			if(payload && payload->Data)
			{
				Entity* draggedEntity = (Entity*) payload->Data;
				auto& dr = draggedEntity->GetOrAddComponent<EntityRelationshipComponent>();
				dr.SetParent(*draggedEntity, entity);
				LOG_INFO("Dropped Entity!");
			}

			ImGui::EndDragDropTarget();
		}

	}

}