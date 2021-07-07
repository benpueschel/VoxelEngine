#pragma once

#include "Voxel/Core/Core.h"

#include "KeyCodes.h"
#include "MouseButtons.h"

#include <glm/glm.hpp>

namespace Voxel {

	class Input
	{

	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButton button);
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();

	};

}