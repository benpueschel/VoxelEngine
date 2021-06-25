#pragma once

#include "Voxel/Rendering/Materials/Texture.h"
#include <glad/glad.h>

#include <string>

#include <glm/glm.hpp>

namespace Voxel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }		
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual glm::vec2& GetScale() override { return m_Scale; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetScale(const glm::vec2& scale) override { m_Scale = scale; }

		virtual void SetWrapMode(const TextureWrapMode& wrapMode) const override;
		virtual void SetFilterMode(const TextureFilterMode& filterMode) const override;

		virtual void Bind(uint32_t slot) const override; 

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; 
		}

	private:
		std::string m_Path;

		uint32_t m_Width;
		uint32_t m_Height;
		glm::vec2 m_Scale = glm::vec2(1.0f);

		uint32_t m_RendererID;

		GLenum m_DataFormat;
		GLenum m_InternalFormat;

	};

}