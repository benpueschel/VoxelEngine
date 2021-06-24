#pragma once

#include "RenderCommand.h"
#include "Camera/Camera.h"

#include "Shaders/Shader.h"

#include "Voxel/Events/ApplicationEvent.h"

namespace Voxel {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowFramebufferResize(WindowFramebufferResizeEvent& event);

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			Transform& = Transform()
		);

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;

	};

}