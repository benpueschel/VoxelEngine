#pragma once

#include "RenderCommand.h"
#include "Camera.h"

#include "Shaders/Shader.h"

#include "Voxel/Events/ApplicationEvent.h"

namespace Voxel {

	class Entity;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowRestore(WindowRestoreEvent& event);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void Submit(
			Entity& entity,
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray
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