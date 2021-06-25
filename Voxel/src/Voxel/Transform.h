#pragma once

#include <glm/glm.hpp>

namespace Voxel {

	class Transform
	{
	public:
		Transform();
		// TODO: Only optimize the matrix at the end of every frame, as it will be redundant when calling 2 methods in the same frame.
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateMatrix();
		}
		void SetRotation(const glm::vec3& rotation)
		{
			m_Rotation = rotation;
			RecalculateMatrix();
		}
		void SetScale(const glm::vec3& scale)
		{
			m_Scale = scale;
			RecalculateMatrix();
		}
		void Rotate(const glm::vec3& rotation)
		{
			SetRotation(GetRotation() + rotation);
		}

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetRotation() { return m_Rotation; }
		glm::mat4& GetTransform() { return m_Transform; }
		glm::vec3& GetScale() { return m_Scale; }

		glm::vec3 Forward();
		glm::vec3 Right();
		glm::vec3 Up();

		static glm::vec3 Rotate(const glm::vec3& vector, const glm::vec3& rotation);
		static glm::mat4 Rotate(const glm::mat4& vector, const glm::vec3& rotation);

	private:
		void RecalculateMatrix();

	private:
		glm::vec3 m_Position = { 0, 0, 0 };
		glm::vec3 m_Rotation = { 0, 0, 0 };
		glm::vec3 m_Scale = { 1, 1, 1 };

		glm::mat4 m_Transform;
	};

}