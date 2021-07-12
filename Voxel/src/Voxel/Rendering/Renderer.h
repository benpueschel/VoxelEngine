#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "EditorCamera.h"

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
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		void Submit(Entity& entity);
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