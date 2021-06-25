#include <pch.h>

#include "Voxel/Input/Input.h"
#include "Voxel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Voxel {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		const auto& window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		const auto& window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		const auto& window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

}