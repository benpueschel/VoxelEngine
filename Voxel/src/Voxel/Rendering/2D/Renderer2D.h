#pragma once

#include "Voxel/Transform.h"

#include "Voxel/Rendering/Camera/OrthographicCamera.h"
#include "Voxel/Rendering/Materials/Texture.h"

namespace Voxel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitive Submitting
		static void DrawQuad(Transform& transform, const glm::vec4& color);
		static void DrawQuad(Transform& transform, const Ref<Texture2D>& texture);
		static void DrawQuad(Transform& transform, const Ref<Texture2D>& texture, const glm::vec4& color);
	};

}