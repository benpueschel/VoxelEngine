#include <pch.h>
#include "Buffer.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/Shaders/OpenGLBuffer.h"

namespace Voxel {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

}