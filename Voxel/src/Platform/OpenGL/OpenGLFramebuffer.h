#pragma once

#include "Voxel/Rendering/Framebuffer.h"

namespace Voxel {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() override;
		virtual void Unbind() override;
		
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};

}