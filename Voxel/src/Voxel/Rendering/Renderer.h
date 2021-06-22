#pragma once

#include "RenderCommand.h"
#include "Camera/Camera.h"

#include "Shaders/Shader.h"

namespace Voxel {

	class Renderer
	{
	public:

		static void Init();

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> m_SceneData;

	};

}