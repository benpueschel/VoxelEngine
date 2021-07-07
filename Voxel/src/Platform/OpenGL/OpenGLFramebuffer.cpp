#include <pch.h>
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Voxel {

	static bool IsDepthFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::DEPTH24_STENCIL8:
				return true;
		}

		return false;
	}

	static GLenum TextureTarget(bool multisample)
	{
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisample, uint32_t* data, uint32_t size)
	{
		glCreateTextures(TextureTarget(multisample), size, data);
	}

	static void BindTexture(bool multisample, uint32_t id)
	{
		glBindTexture(TextureTarget(multisample), id);
	}

	static void AttachColorTexture(uint32_t id, uint32_t samples, GLenum internalFormat, 
		GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisample = samples > 1;
		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);

			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, uint32_t samples, GLenum format, GLenum type, uint32_t width, uint32_t height, int index)
	{
		bool multisample = samples > 1;
		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);

			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, type, TextureTarget(multisample), id, 0);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_Specification(specification)
	{
		// Attachments
		for (const auto& format : m_Specification.Attachments)
		{
			if (!IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentSpecs.emplace_back(format);
			else
				m_ColorAttachmentSpecs.emplace_back(format);
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(m_DepthAttachments.size(), m_DepthAttachments.data());

		m_ColorAttachments.clear();
		m_DepthAttachments.clear();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(m_DepthAttachments.size(), m_DepthAttachments.data());

			m_ColorAttachments.clear();
			m_DepthAttachments.clear();
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;
		if (!m_ColorAttachmentSpecs.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());

			CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples,
							GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples,
							GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
						break;
				}
			}
		}
		
		if (!m_DepthAttachmentSpecs.empty())
		{
			CreateTextures(multisample, m_DepthAttachments.data(), m_DepthAttachments.size());
			for (size_t i = 0; i < m_DepthAttachments.size(); i++)
			{
				BindTexture(multisample, m_DepthAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
					case FramebufferTextureFormat::DEPTH24_STENCIL8:
						AttachDepthTexture(m_DepthAttachments[i], m_Specification.Samples,
							GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 
							m_Specification.Width, m_Specification.Height, i);
						break;
				}
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			CORE_ASSERT((m_ColorAttachments.size() <= 4), "Framebuffer cannot have more than 4 texture attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		} 
		else if(m_ColorAttachments.empty()) 
		{
			glDrawBuffer(GL_NONE);
		}

		CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Could not complete Framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		CORE_ASSERT((attachmentIndex < m_ColorAttachments.size()), "Index out of bounds");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int data;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);

		return data;
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}