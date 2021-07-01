#pragma once

#include "Voxel/Rendering/Camera.h"
#include <glm/glm.hpp>

namespace Voxel {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType : uint32_t { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		SceneCamera(const glm::mat4& projection);
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, const glm::vec2& clipingPlane);
		void SetPerspective(float fov, const glm::vec2& clipingPlane);
		void SetProjectionType(ProjectionType type);

		void SetViewportSize(uint32_t width, uint32_t height);
		void RecalculateProjection();

		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; }
		void SetPerspectiveClippingPlane(glm::vec2& plane) { m_PerspectiveClippingPlane = plane; }

		void SetOrthographicSize(float size) { m_OrthographicSize = size; }
		void SetOrthographicClippingPlane(glm::vec2& plane) { m_OrthographicClippingPlane = plane; }

		ProjectionType& GetProjectionType() { return m_ProjectionType; }
		const ProjectionType& GetProjectionType() const { return m_ProjectionType; }

		float& GetPerspectiveFOV() { return m_PerspectiveFOV; }
		glm::vec2& GetPerspectiveClippingPlane() { return m_PerspectiveClippingPlane; }

		float& GetOrthographicSize() { return m_OrthographicSize; }
		glm::vec2& GetOrthographicClippingPlane() { return m_OrthographicClippingPlane; }

		glm::vec2& GetClippingPlane()
		{
			if (m_ProjectionType == ProjectionType::Orthographic)
				return m_OrthographicClippingPlane;
			return m_PerspectiveClippingPlane;
		}

	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		float m_PerspectiveFOV = 90.0f;
		float m_OrthographicSize = 10.0f;
		float m_AspectRatio = 0.0f;
		glm::vec2 m_PerspectiveClippingPlane{ 0.001f, 10000.0f };
		glm::vec2 m_OrthographicClippingPlane{ -100.0f, 100.0f };
	};

}