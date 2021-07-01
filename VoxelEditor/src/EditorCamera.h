#pragma once

#include "Voxel.h"

namespace Voxel {

	class EditorCamera : public ScriptableEntity
	{
	protected:
		virtual void OnUpdate(Timestep timestep)
		{
			auto& transform = GetComponent<TransformComponent>();

			glm::vec3 move = glm::vec3(0, 0, 0);

			if (Input::IsKeyPressed(KeyCode::W))
				move += glm::vec3(0, 0, -m_MoveSpeed);
			if (Input::IsKeyPressed(KeyCode::S))
				move += glm::vec3(0, 0, m_MoveSpeed);
			if (Input::IsKeyPressed(KeyCode::A))
				move += glm::vec3(-m_MoveSpeed, 0, 0);
			if (Input::IsKeyPressed(KeyCode::D))
				move += glm::vec3(m_MoveSpeed, 0, 0);

			move *= timestep;

			transform.LocalPosition += move;
		}

	private:
		float m_MoveSpeed = 1.0f;

	};

}