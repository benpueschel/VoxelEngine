#pragma once

#include "Voxel/Rendering/Camera/OrthographicCamera.h"
#include "Voxel/Core/Timestep.h"

#include "Voxel/Events/ApplicationEvent.h"
#include "Voxel/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Voxel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);

		void OnUpdate(Timestep& timestep);
		void OnEvent(Event& event);

		Camera& GetCamera() { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
	};

}