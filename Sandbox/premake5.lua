project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{IncludeDir.spdlog}",
		"%{wks.location}/Voxel/src",
		"%{wks.location}/Voxel/vendor",
		"%{IncludeDir.glm}",
	}

	links {
		"Voxel"
	}

	filter "system:windows"
		systemversion "latest"

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