#pragma once

#include "Voxel/Core/Core.h"

#include "KeyCodes.h"
#include "MouseButtons.h"

namespace Voxel {

	class Input
	{

	public:
		inline static bool IsKeyPressed(KeyCode keycode)
		{
			return s_Instance->IsKeyPressedImpl(keycode);
		}

		inline static bool IsMouseButtonPressed(MouseButton button)
		{
			return s_Instance->IsMouseButtonPressedImpl(button);
		}

		inline static float GetMouseX()
		{
			return s_Instance->GetMouseXImpl();
		}

		inline static float GetMouseY()
		{
			return s_Instance->GetMouseYImpl();
		}
		inline static std::pair<float, float> GetMousePos()
		{
			return s_Instance->GetMousePosImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButton button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;

	private:
		static Scope<Input> s_Instance;

	};

}