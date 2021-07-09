#pragma once

#include "Voxel/Rendering/Framebuffer.h"

namespace Voxel {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual const std::vector<uint32_t>& GetColorAttachments() const override { return m_ColorAttachments; }
		virtual const std::vector<uint32_t>& GetDepthAttachments() const override { return m_DepthAttachments; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual void Bind() override;
		virtual void Unbind() override;
		
	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		std::vector<FramebufferTextureSpecification> m_DepthAttachmentSpecs;

		std::vector<uint32_t> m_ColorAttachments;
		std::vector<uint32_t> m_DepthAttachments;
	};

}