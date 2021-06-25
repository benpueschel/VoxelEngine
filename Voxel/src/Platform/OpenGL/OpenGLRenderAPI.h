#pragma once

#include "Voxel/Rendering/RenderAPI.h"
#include "Voxel/Core/Core.h"

namespace Voxel {

	class OpenGLRenderAPI : public RenderAPI
	{

		virtual void Init() override;
		virtual void SetViewport(glm::ivec4 dimensions) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

	};

}