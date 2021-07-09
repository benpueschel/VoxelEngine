#pragma once

#include "Voxel/Rendering/Camera.h"
#include "Voxel/Rendering/Materials/Texture.h"
#include "Voxel/Rendering/EditorCamera.h"

#include "Voxel/Scene/Components/Components.h"

namespace Voxel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderer, int entityID);
		// Primitive Submitting
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID = -1);
	};

}