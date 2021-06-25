#pragma once

#include "RenderAPI.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Voxel {

	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void SetViewport(glm::ivec4 dimensions)
		{
			s_RenderAPI->SetViewport(dimensions);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RenderAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};

}