#include <pch.h>
#include "UniformBuffer.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/Shaders/OpenGLUniformBuffer.h"

namespace Voxel {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}