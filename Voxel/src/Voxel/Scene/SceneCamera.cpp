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
		m_Orthographic = true;
		m_OrthographicSize = size;
		m_ClippingPlane = clipingPlane;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, const glm::vec2& clipingPlane)
	{
		m_Orthographic = false;
		m_PerspectiveFOV = fov;
		m_ClippingPlane = clipingPlane;
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
		if (m_Orthographic)
		{
			float orthoWidth = m_OrthographicSize * m_AspectRatio * 0.5f;
			float ortoHeight = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(-orthoWidth, orthoWidth, -ortoHeight, ortoHeight, m_ClippingPlane.x, m_ClippingPlane.y);
		}
		else
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_ClippingPlane.x, m_ClippingPlane.y);
		}
	}

}