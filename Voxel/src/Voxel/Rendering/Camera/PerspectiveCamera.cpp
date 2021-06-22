#include <pch.h>
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Voxel {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio)
		: m_ProjectionMatrix(glm::perspective(fov, aspectRatio, 1.0f, 150.0f)), m_ViewMatrix(1.0f),
		m_Position(0.0f), m_Rotation(0.0f)
	{

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix(float fov, float aspectRatio)
	{
		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, 0.01f, 150.0f);
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(glm::length(m_Rotation)), glm::normalize(m_Rotation));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}