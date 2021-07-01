#include <pch.h>
#include "SceneCamera.h"

#include <glm/gtx/transform.hpp>

namespace Voxel {

	SceneCamera::SceneCamera() 
	{
		RecalculateProjection();
	}

	SceneCamera::SceneCamera(const glm::mat4& projection)
		: Camera(projection) 
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, const glm::vec2& clipingPlane)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicClippingPlane = clipingPlane;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, const glm::vec2& clipingPlane)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = fov;
		m_PerspectiveClippingPlane = clipingPlane;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		float aspectRatio = (float)width / (float)height;
		if (m_AspectRatio != aspectRatio)
		{
			m_AspectRatio = aspectRatio;
			RecalculateProjection();
		}
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoWidth = m_OrthographicSize * m_AspectRatio * 0.5f;
			float ortoHeight = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(-orthoWidth, orthoWidth, -ortoHeight, ortoHeight, 
				m_OrthographicClippingPlane.x, m_OrthographicClippingPlane.y);
		}
		else if(m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, 
				m_PerspectiveClippingPlane.x, m_PerspectiveClippingPlane.y);
		}
	}

}