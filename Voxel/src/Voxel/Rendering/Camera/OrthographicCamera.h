#pragma once

#include "Camera.h"

namespace Voxel {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
		}

		void SetRotation(glm::vec3 rotation)
		{
			m_Rotation = rotation;
		}

		void SetProjection(float left, float right, float bottom, float top);
		void RecalculateViewMatrix();

		virtual const glm::vec3& GetPosition() const override { return m_Position; }
		virtual const glm::vec3 GetRotation() const override { return m_Rotation; }

		virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.01f, 0.0f };

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}