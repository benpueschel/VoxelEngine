#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Voxel/Scene/SceneCamera.h"
#include "Voxel/Scene/ScriptableEntity.h"

namespace Voxel {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) 
			: Tag(tag) { }

		void OnImGuiRender();
	};

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

		glm::mat4 GetTransform()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), LocalRotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), LocalRotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), LocalRotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), LocalPosition)
				* rotation
				* glm::scale(glm::mat4(1.0f), LocalScale);
		}

		operator glm::mat4& () { return GetTransform(); }

		void OnImGuiRender();
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }

		void OnImGuiRender();
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void OnImGuiRender();
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {
				return static_cast<ScriptableEntity*>(new T());
			};
			DestroyScript = [](NativeScriptComponent* script) {
				delete script;
				script->Instance = nullptr;
			};
		}

		void OnImGuiRender();
	};

}