#include <pch.h>
#include "VertexArray.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/Shaders/OpenGLVertexArray.h"

namespace Voxel {

	Ref<VertexArray> VertexArray::Create()
	{

		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL: 
				return CreateRef<OpenGLVertexArray>();
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

}