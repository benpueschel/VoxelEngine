#include <pch.h>
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Voxel {

	Transform::Transform()
	{
		RecalculateMatrix();
	}

	void Transform::RecalculateMatrix()
	{
		m_Transform = Rotate(glm::scale(glm::translate(glm::mat4(1.0f), m_Position), m_Scale),  m_Rotation);
	}

	glm::vec3 Transform::Forward()
	{
		return Rotate({ 0, 0, 1 }, m_Rotation);
	}

	glm::vec3 Transform::Right()
	{
		return Rotate({ 1, 0, 0 }, m_Rotation);
	}

	glm::vec3 Transform::Top()
	{
		return Rotate({ 0, 1, 0 }, m_Rotation);
	}

	glm::vec3 Transform::Rotate(const glm::vec3& vector, const glm::vec3& rotation)
	{
		if (rotation == glm::vec3(0.0f))
			return vector;

		glm::vec3 rotatedVector = vector;
		rotatedVector = glm::rotateX(rotatedVector, glm::radians(rotation.x));
		rotatedVector = glm::rotateY(rotatedVector, glm::radians(rotation.y));
		rotatedVector = glm::rotateZ(rotatedVector, glm::radians(rotation.z));

		return rotatedVector;
	}

	glm::mat4 Transform::Rotate(const glm::mat4& matrix, const glm::vec3& rotation)
	{
		if (rotation == glm::vec3(0.0f))
			return matrix;

		return glm::rotate(matrix, glm::radians(glm::length(rotation)), glm::normalize(rotation));
	}

}