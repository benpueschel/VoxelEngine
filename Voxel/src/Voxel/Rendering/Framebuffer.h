#pragma once

#include "Voxel.h"

namespace Voxel {

	enum class FramebufferTextureFormat : uint32_t
	{
		None = 0,

		// --------- Color ---------
		RGBA8,
		RED_INTEGER,

		// ----- Depth/stencil -----
		DEPTH24_STENCIL8,

		// ------- Defaults --------
		Depth = DEPTH24_STENCIL8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: Filtering/wrap

		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;
		std::vector<FramebufferTextureSpecification> Attachments;

		bool SwapChainTarget = false;
		bool NoResize = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual const std::vector<uint32_t>& GetColorAttachments() const = 0;
		virtual const std::vector<uint32_t>& GetDepthAttachments() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};

}