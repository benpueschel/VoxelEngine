#include <pch.h>
#include "Camera.h"

namespace Voxel {

	void CameraComponent::OnImGuiRender()
	{
		ImGui::Checkbox("Primary", &Primary);

		const char* projections[] = { "Perspective", "Orthographic" };
		const char* currentProjection = projections[static_cast<uint32_t>(Camera.GetProjectionType())];
		if (ImGui::BeginCombo("Projection", currentProjection))
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

}