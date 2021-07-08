#include <pch.h>
#include "SpriteRenderer.h"

namespace Voxel {

	void SpriteRendererComponent::OnImGuiRender()
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(Color));
	}

}