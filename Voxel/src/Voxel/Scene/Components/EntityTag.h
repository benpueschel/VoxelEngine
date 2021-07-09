#pragma once

#include <string>
#include <imgui.h>
#include "misc/cpp/imgui_stdlib.h"

namespace Voxel {

	struct EntityTagComponent
	{
		std::string Tag;

		EntityTagComponent() = default;
		EntityTagComponent(const EntityTagComponent&) = default;
		EntityTagComponent(const std::string& tag)
			: Tag(tag) { }

		void OnImGuiRender();
	};

}