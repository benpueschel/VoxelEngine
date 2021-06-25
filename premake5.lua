include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "VoxelEngine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"

	include "vendor/premake"
	include "Voxel/vendor/glfw"
	include "Voxel/vendor/GLAD"
	include "Voxel/vendor/ImGui"

group ""

include "Voxel"
include "VoxelEditor"
include "Sandbox"
include "VulkanTest"