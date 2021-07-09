#include <pch.h>
#include "EntityTag.h"

namespace Voxel {

	void EntityTagComponent::OnImGuiRender()
	{
		ImGui::SameLine();
		ImGui::InputText("##Tag", &Tag);
	}

}