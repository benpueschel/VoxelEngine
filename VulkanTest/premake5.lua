project "VulkanTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{IncludeDir.spdlog}",
		"%{wks.location}/Voxel/src",
		"%{wks.location}/Voxel/vendor",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImTerm}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.VulkanSDK}"
	}

	links {
		"Voxel"
	}

	filter "system:windows"
		systemversion "latest"

		defines "GLFW_INCLUDE_VULKAN"

	filter "configurations:Debug"
		defines {
			"VOXEL_DEBUG",
			"ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VOXEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VOXEL_DIST"
		runtime "Release"
		optimize "on"