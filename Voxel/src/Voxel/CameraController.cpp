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
		PROFILE_FUNCTION();
		float cameraSpeed = 2 * timestep * m_ZoomLevel * 0.5f;
		float cameraRotationSpeed = 90 * timestep * m_ZoomLevel * 0.5f;

		glm::vec3 move = glm::vec3(0.0f);

		//if (Input::IsKeyPressed(KeyCode::W))
		//	move += m_Camera.GetTransform().Forward();
		//if (Input::IsKeyPressed(KeyCode::S))
		//	move -= m_Camera.GetTransform().Forward();
		//if (Input::IsKeyPressed(KeyCode::A))
		//	move -= m_Camera.GetTransform().Right();
		//if (Input::IsKeyPressed(KeyCode::D))
		//	move += m_Camera.GetTransform().Right();
		
		if (Input::IsKeyPressed(KeyCode::W))
			move += m_Camera.GetTransform().Up();
		if (Input::IsKeyPressed(KeyCode::S))
			move -= m_Camera.GetTransform().Up();
		if (Input::IsKeyPressed(KeyCode::A))
			move -= m_Camera.GetTransform().Right();
		if (Input::IsKeyPressed(KeyCode::D))
			move += m_Camera.GetTransform().Right();

		if (Input::IsKeyPressed(KeyCode::Q))
			m_Camera.Rotate({ 0, 0, cameraRotationSpeed });
		if (Input::IsKeyPressed(KeyCode::E))
			m_Camera.Rotate({ 0, 0, -cameraRotationSpeed });

		move *= cameraSpeed;

		m_Camera.SetPosition(move + m_Camera.GetTransform().GetPosition());
		m_Camera.RecalculateViewMatrix();
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		PROFILE_FUNCTION();
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		PROFILE_FUNCTION();
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = glm::max(0.1f, m_ZoomLevel);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		PROFILE_FUNCTION();
		OnResize((float) event.GetWidth(), (float) event.GetHeight());
		return false;
	}

}