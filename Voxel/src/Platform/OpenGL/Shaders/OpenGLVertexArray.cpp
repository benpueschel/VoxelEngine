#include <pch.h>
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Voxel {

	static GLenum GLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3x3: return GL_FLOAT;
			case ShaderDataType::Mat4x4: return GL_FLOAT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			case ShaderDataType::Bool:   return GL_BOOL;
		}

		CORE_ASSERT(false, "Unknown ShaderDataType: {0}", type);

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(m_RendererID);
		buffer->Bind();

		uint32_t index = 0;
		const auto& layout = buffer->GetLayout();

		for (const auto& element : layout)
		{
			GLenum baseType = GLBaseType(element.Type);
			switch (baseType)
			{
				case GL_INT:
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(
						index,
						element.GetComponentCount(),
						GLBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset
					);
					break;

				default:
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						element.GetComponentCount(),
						GLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset
					);
					break;
			}
			index++;
		}

		m_VertexBuffers.push_back(buffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		buffer->Bind();

		m_IndexBuffer = buffer;
	}

}