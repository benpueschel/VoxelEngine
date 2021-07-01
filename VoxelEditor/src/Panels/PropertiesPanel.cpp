#include "PropertiesPanel.h"

#include <imgui.h>

namespace Voxel {

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");
		if (m_Context)
		{

			const static auto treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_FramePadding
				| ImGuiTreeNodeFlags_AllowItemOverlap;

			const static auto transformTreeNodeFlags = 
				ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_FramePadding
				| ImGuiTreeNodeFlags_AllowItemOverlap;

			if (m_Context.HasComponent<TagComponent>())
				m_Context.GetComponent<TagComponent>().OnImGuiRender();

			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_Context.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_Context.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::PopItemWidth();

			DrawComponent<TransformComponent>("Transform", transformTreeNodeFlags, false);
			DrawComponent<CameraComponent>("Camera", treeNodeFlags);
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", treeNodeFlags);

		}
		ImGui::End();
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