#pragma once

#include "Voxel/Core/Core.h"

#include "KeyCodes.h"
#include "MouseButtons.h"

namespace Voxel {

	class Input
	{

	public:
		static bool IsKeyPressed(KeyCode keycode);

		inline static bool IsMouseButtonPressed(MouseButton button);
		inline static float GetMouseX();
		inline static float GetMouseY();
		inline static std::pair<float, float> GetMousePosition();

	};

}