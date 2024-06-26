#pragma once

#include <glm/glm.hpp>

#include "Voxel/Rendering/Shaders/VertexArray.h"

namespace Voxel {

	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Vulkan = 2,
		};

	public:

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SetViewport(glm::ivec4 dimensions) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static inline API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}