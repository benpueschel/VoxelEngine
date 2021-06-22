#pragma once

#include "Voxel/Rendering/GraphicsContext.h"
#include "vulkan/vulkan.h"

struct GLFWwindow;

namespace Voxel {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow * windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Destroy() override;

	private:
		static void InitializeVulkan();

	private:
		GLFWwindow* m_WindowHandle;
		VkInstance m_Instance;

		static VkInstanceCreateInfo s_CreateInfo;
		static bool s_VulkanInitialized;
	};
}
