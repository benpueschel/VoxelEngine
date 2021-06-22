#pragma once

#include "Voxel/Rendering/RenderAPI.h"

namespace Voxel {

	class OpenGLRenderAPI : public RenderAPI
	{

		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}