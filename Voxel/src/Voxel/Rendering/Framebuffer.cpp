#include <pch.h>
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Voxel {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:
				return CreateRef<OpenGLFramebuffer>(specification);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

}