#pragma once

#include "Voxel/Rendering/GraphicsContext.h"

struct GLFWwindow;

namespace Voxel {

	class OpenGLContext : public GraphicsContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Destroy() override;

	private:
		GLFWwindow* m_WindowHandle;

	};

}