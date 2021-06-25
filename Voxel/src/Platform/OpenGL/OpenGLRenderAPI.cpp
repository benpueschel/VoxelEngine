#include <pch.h>
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace Voxel {

	void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_FATAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
		}

		CORE_ASSERT(false, "Unknown severity level");
	}

	void OpenGLRenderAPI::Init()
	{
		PROFILE_FUNCTION();

	#ifdef VOXEL_DEBUG 
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::SetViewport(glm::ivec4 dimensions)
	{
		glViewport(dimensions.x, dimensions.y, dimensions.z, dimensions.w);
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}