#include <pch.h>
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGlRenderAPI.h"

namespace Voxel {

	Scope<RenderAPI> RenderCommand::s_RenderAPI = CreateScope<OpenGLRenderAPI>();

}