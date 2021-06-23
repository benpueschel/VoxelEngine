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

		if (Input::IsKeyPressed(KeyCode::KEY_W))
			m_Position.y += cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_S))
			m_Position.y -= cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_A))
			m_Position.x -= cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_D))
			m_Position.x += cameraSpeed;

		if (Input::IsKeyPressed(KeyCode::KEY_Q))
			m_Rotation.y += cameraRotationSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_E))
			m_Rotation.y -= cameraRotationSpeed;

		m_Camera.SetPosition(m_Position);
		m_Camera.SetRotation(m_Rotation);

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