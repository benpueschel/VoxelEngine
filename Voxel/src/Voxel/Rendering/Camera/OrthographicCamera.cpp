#include <pch.h>
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/normalize_dot.hpp>

namespace Voxel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -100.0f, 100.0f)), m_ViewMatrix(1.0f),
		m_Position(0.0f), m_Rotation(0.0f)
	{

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.0f, 100.0f);
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(glm::length(m_Rotation)), glm::normalize(m_Rotation));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}