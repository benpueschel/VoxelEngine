#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Voxel {

	static GLenum GLWrapMode(const TextureWrapMode& wrapMode)
	{
		switch (wrapMode)
		{
			case TextureWrapMode::CLAMP:
				return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::MIRROR:
				return GL_MIRRORED_REPEAT;
			case TextureWrapMode::REPEAT:
				return GL_REPEAT;
			default:
				break;
		}
		CORE_ASSERT(false, "Unknown TextureWrapMode. Using TextureWrapMode::REPEAT instead");
		return GL_REPEAT;
	}

	static GLenum GLFilterMode(const TextureFilterMode& filterMode)
	{
		switch (filterMode)
		{
			case TextureFilterMode::POINT:
				return GL_NEAREST;
			case TextureFilterMode::BILLINEAR:
				return GL_LINEAR;
			case TextureFilterMode::TRILLINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
		}
		CORE_ASSERT(false, "Unknown TextureFilterMode. Using TextureFilterMode::BILLINEAR instead");
		return GL_LINEAR;
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_DataFormat(GL_RGBA), m_InternalFormat(GL_RGBA8)
	{
		PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		SetWrapMode(TextureWrapMode::REPEAT);
		SetFilterMode(TextureFilterMode::BILLINEAR);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		PROFILE_FUNCTION();

		int width, height, channels;
		// OpenGL reads Textures from bottom to top, so flip the the data on load
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D(const std::string&) - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		CORE_ASSERT(data, "Could not load image at path {0}", path);

		switch (channels)
		{
			case 3:
				m_DataFormat = GL_RGB;
				m_InternalFormat = GL_RGB8;
				break;
			case 4:
				m_DataFormat = GL_RGBA;
				m_InternalFormat = GL_RGBA8;
				break;
		}

		CORE_ASSERT(m_DataFormat && m_InternalFormat, "Texture format not supported.");

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		SetWrapMode(TextureWrapMode::REPEAT);
		SetFilterMode(TextureFilterMode::BILLINEAR);

		SetData(data, sizeof(data));

		stbi_image_free(data);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		PROFILE_FUNCTION();

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::SetWrapMode(const TextureWrapMode& wrapMode) const
	{
		PROFILE_FUNCTION();

		GLenum glWrapMode = GLWrapMode(wrapMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glWrapMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, glWrapMode);
	}

	void OpenGLTexture2D::SetFilterMode(const TextureFilterMode& filterMode) const
	{
		PROFILE_FUNCTION();

		GLenum glFilterMode = GLFilterMode(filterMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, glFilterMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, glFilterMode);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

}