#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Voxel/Scene/SceneCamera.h"

namespace Voxel {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) 
			: Tag(tag) { }
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) 
			: Transform(transform) { }

		const glm::vec3& GetPosition() const { return glm::vec3(Transform[3]); }
		glm::vec3& GetPosition() { return glm::vec3(Transform[3]); }
		glm::vec4& MGetPosition() { return Transform[3]; }

		void SetPosition(glm::vec3& position) { Transform[3] = glm::vec4(position, 1); }

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& const () { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}