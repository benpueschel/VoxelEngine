#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Voxel {

	class PerspectiveCamera : public Camera
	{

	public:
		PerspectiveCamera(float fov, float aspectRatio);

		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}

		void SetRotation(glm::vec3 rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		virtual const glm::vec3& GetPosition() const override { return m_Position; }
		const glm::vec3 GetRotation() const { return m_Rotation; }

		virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		void RecalculateProjectionMatrix(float fov, float aspectRatio);

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
	};

}