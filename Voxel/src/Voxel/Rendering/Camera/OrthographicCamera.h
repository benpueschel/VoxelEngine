#pragma once

#include "Camera.h"
#include "Voxel/Transform.h"

namespace Voxel {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);
		void RecalculateViewMatrix();

		virtual const void SetPosition(const glm::vec3& position)
		{
			m_Transfrom.SetPosition(position);
		}
		virtual const void SetRotation(const glm::vec3& rotatiopn)
		{
			m_Transfrom.SetPosition(rotatiopn);
		}
		const void Rotate(const glm::vec3& rotation)
		{
			m_Transfrom.SetRotation(m_Transfrom.GetRotation() + rotation);
		}

		virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		virtual Transform& GetTransform() override { return m_Transfrom; }

	private:
		Transform m_Transfrom;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}