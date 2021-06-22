#include <pch.h>
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Voxel {



	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CORE_ASSERT(m_WindowHandle, "Could not initialize OpenGLContext: m_WindowHandle does not exist");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(status, "Failed to initialize GLAD");

		LOG_CORE_INFO("OpenGL Info: ");
		LOG_CORE_INFO("  Vendor: {0}", glad_glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glad_glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version: {0}", glad_glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::Destroy() {}

}