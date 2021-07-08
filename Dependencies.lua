VULKAN_SDK = os.getenv("VULKAN_SDK")
VOXEL_VENDOR = "%{wks.location}/Voxel/vendor"

IncludeDir = {}
IncludeDir["spdlog"]			= "%{VOXEL_VENDOR}/spdlog/include"
IncludeDir["GLFW"] 				= "%{VOXEL_VENDOR}/glfw/include"
IncludeDir["glm"] 				= "%{VOXEL_VENDOR}/glm"
IncludeDir["stb_image"]			= "%{VOXEL_VENDOR}/stb_image"
IncludeDir["GLAD"] 				= "%{VOXEL_VENDOR}/GLAD/include"
IncludeDir["ImGui"] 			= "%{VOXEL_VENDOR}/ImGui"
IncludeDir["ImTerm"] 			= "%{VOXEL_VENDOR}/ImTerm/include"
IncludeDir["entt"]				= "%{VOXEL_VENDOR}/entt/include"
IncludeDir["yaml_cpp"] 			= "%{VOXEL_VENDOR}/yaml-cpp/include"
IncludeDir["ImGuizmo"] 			= "%{VOXEL_VENDOR}/ImGuizmo"
IncludeDir["VulkanSDK"]			= "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"]			= "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"]	= "%{VOXEL_VENDOR}/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] 				= "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] 			= "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"