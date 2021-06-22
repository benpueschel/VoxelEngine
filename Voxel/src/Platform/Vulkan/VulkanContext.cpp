#include <pch.h>
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Voxel {

	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CORE_ASSERT(m_WindowHandle, "Could not initialize VulkanContext: m_WindowHandle does not exist");
	}

	void VulkanContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		if (!s_VulkanInitialized)
			InitializeVulkan();

		uint32_t extensionCount = 0;

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		LOG_CORE_DEBUG("Supported extensions:");

		for (const auto& extension : extensions)
		{
			LOG_CORE_DEBUG("  {0}", extension.extensionName);
		}

		VkResult result = vkCreateInstance(&s_CreateInfo, nullptr, &m_Instance);
		
		CORE_ASSERT((result == VK_SUCCESS), "Could not create Vulkan instance: " + result);

	}

	void VulkanContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void VulkanContext::Destroy()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanContext::InitializeVulkan()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "VoxelEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		s_CreateInfo = VkInstanceCreateInfo();
		s_CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		s_CreateInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		s_CreateInfo.enabledExtensionCount = glfwExtensionCount;
		s_CreateInfo.ppEnabledExtensionNames = glfwExtensions;

		s_CreateInfo.enabledLayerCount = 0;
	}

}