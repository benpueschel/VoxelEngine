#include <pch.h>
#include "Components.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include "Voxel/ImGui/UILibrary.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Voxel {

	void TagComponent::OnImGuiRender()
	{
		ImGui::SameLine();
		ImGui::InputText("##Tag", &Tag);
	}

	void TransformComponent::OnImGuiRender()
	{
		UI::DrawVec3Control("Position", LocalPosition);
		
		glm::vec3 rotation = glm::degrees(LocalRotation);
		UI::DrawVec3Control("Rotation", rotation);
		LocalRotation = glm::radians(rotation);

		UI::DrawVec3Control("Scale", LocalScale, 1.0f, 0.001f);
	}

	void SpriteRendererComponent::OnImGuiRender()
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(Color));
	}

	void CameraComponent::OnImGuiRender()
	{
		ImGui::Checkbox("Primary", &Primary);

		const char* projections[] = { "Perspective", "Orthographic" };
		const char* currentProjection = projections[static_cast<uint32_t>(Camera.GetProjectionType())];
		if(ImGui::BeginCombo("Projection", currentProjection))
		{
			for (uint32_t i = 0; i < sizeof(projections) / sizeof(projections[0]); i++)
			{
				bool isSelected = projections[i] == currentProjection;
				if (ImGui::Selectable(projections[i], isSelected))
				{
					currentProjection = projections[i];
					Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();
		}

		switch (Camera.GetProjectionType())
		{
			case SceneCamera::ProjectionType::Perspective:
				if (ImGui::DragFloat("FOV", &Camera.GetPerspectiveFOV(), 0.01f, 15, 180))
					Camera.RecalculateProjection();
				if (ImGui::DragFloat2("Clipping Plane", glm::value_ptr(Camera.GetClippingPlane()), 0.01f, 0.001f, 10000))
					Camera.RecalculateProjection();
				break;
			case SceneCamera::ProjectionType::Orthographic:
				if (ImGui::DragFloat("Size", &Camera.GetOrthographicSize(), 0.01f, 0.01f))
					Camera.RecalculateProjection();
				if (ImGui::DragFloat2("Clipping Plane", glm::value_ptr(Camera.GetClippingPlane()), 0.01f, -100, 100))
					Camera.RecalculateProjection();
				break;
		}

	}

	void NativeScriptComponent::OnImGuiRender()
	{
	}

}