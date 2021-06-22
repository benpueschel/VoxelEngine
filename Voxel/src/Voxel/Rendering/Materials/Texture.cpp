#include <pch.h>
#include "Texture.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/Materials/OpenGLTexture.h"

namespace Voxel {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

}