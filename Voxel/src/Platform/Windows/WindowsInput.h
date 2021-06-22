#pragma once

#include "Voxel/Input/Input.h"
#include "Voxel/Input/KeyCodes.h"
#include "Voxel/Input/MouseButtons.h"

namespace Voxel {

	class WindowsInput : public Input
	{

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseButton button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};

}