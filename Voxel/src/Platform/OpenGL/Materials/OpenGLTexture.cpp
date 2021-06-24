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

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		CORE_ASSERT(data, "Could not load image at path {0}", path);

		GLenum dataFormat = 0, internalFormat = 0;

		switch (channels)
		{
		case 3:
			dataFormat = GL_RGB;
			internalFormat = GL_RGB8;
			break;
		case 4:
			dataFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		}

		CORE_ASSERT(dataFormat && internalFormat, "Texture format not supported.");

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		SetWrapMode(TextureWrapMode::REPEAT);
		SetFilterMode(TextureFilterMode::BILLINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void OpenGLTexture2D::SetWrapMode(const TextureWrapMode& wrapMode) const
	{
		GLenum glWrapMode = GLWrapMode(wrapMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, glWrapMode);
	}

	void OpenGLTexture2D::SetFilterMode(const TextureFilterMode& filterMode) const
	{
		GLenum glFilterMode = GLFilterMode(filterMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, glFilterMode);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, glFilterMode);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}