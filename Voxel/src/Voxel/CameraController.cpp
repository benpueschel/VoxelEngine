#include <pch.h>
#include "CameraController.h"

#include "Voxel/Input/Input.h"

namespace Voxel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
		
	}

	void OrthographicCameraController::OnUpdate(Timestep& timestep)
	{
		float cameraSpeed = 2 * timestep * m_ZoomLevel * 0.5f;
		float cameraRotationSpeed = 90 * timestep * m_ZoomLevel * 0.5f;

		glm::vec3 move = glm::vec3(0.0f);

		if (Input::IsKeyPressed(KeyCode::KEY_W))
			move += m_Camera.GetTransform().Forward();
		if (Input::IsKeyPressed(KeyCode::KEY_S))
			move -= m_Camera.GetTransform().Forward();
		if (Input::IsKeyPressed(KeyCode::KEY_A))
			move -= m_Camera.GetTransform().Right();
		if (Input::IsKeyPressed(KeyCode::KEY_D))
			move += m_Camera.GetTransform().Right();

		if (Input::IsKeyPressed(KeyCode::KEY_Q))
			m_Camera.Rotate({ 0, cameraRotationSpeed, 0 });
		if (Input::IsKeyPressed(KeyCode::KEY_E))
			m_Camera.Rotate({ 0, -cameraRotationSpeed, 0 });

		move *= cameraSpeed;
		LOG_CORE_DEBUG("{0}, {1}, {2}", move.x, move.y, move.z);

		move += m_Camera.GetTransform().GetPosition();

		m_Camera.SetPosition(move);

		m_Camera.RecalculateViewMatrix();
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = glm::max(0.1f, m_ZoomLevel);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = (float) event.GetWidth() / (float) event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}