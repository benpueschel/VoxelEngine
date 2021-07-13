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
		PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(status, "Failed to initialize GLAD");

		LOG_CORE_INFO("OpenGL Info: ");
		LOG_CORE_INFO("  Vendor: {0}", glad_glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glad_glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version: {0}", glad_glGetString(GL_VERSION));

		CORE_ASSERT((GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5)), 
			"OpenGL Versioon >= 4.5 required");
	}

	void OpenGLContext::SwapBuffers()
	{
		PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::Destroy() {}

}