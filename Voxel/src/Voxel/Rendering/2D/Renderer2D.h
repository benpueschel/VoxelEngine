#pragma once

#include "Voxel/Rendering/Camera.h"
#include "Voxel/Rendering/Materials/Texture.h"

namespace Voxel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();

		// Primitive Submitting
		static void DrawQuad(glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(glm::mat4& transform, const Ref<Texture2D>& texture);
		static void DrawQuad(glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color);
	};

}