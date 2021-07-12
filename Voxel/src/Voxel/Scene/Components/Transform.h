#pragma once

#include <glm/glm.hpp>
#include "Voxel/ImGui/UILibrary.h"

namespace Voxel {

	struct TransformComponent
	{
		glm::vec3 LocalPosition{ 0.0f };
		glm::vec3 LocalRotation{ 0.0f };
		glm::vec3 LocalScale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		void SetLocalPosition(glm::vec3& position) { LocalPosition = position; }
		void SetLocalRotation(glm::vec3& rotation) { LocalRotation = glm::radians(rotation); }
		void SetLocalScale(glm::vec3& scale) { LocalScale = scale; }

		glm::mat4 GetTransform();

		operator glm::mat4 () { return GetTransform(); }
		operator glm::mat4& () { return GetTransform(); }

		void OnImGuiRender();
	};

}