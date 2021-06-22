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

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};

}