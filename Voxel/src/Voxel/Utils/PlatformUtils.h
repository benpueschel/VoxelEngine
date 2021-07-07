#pragma once

#include <string>
#include <filesystem>
#include <optional>

namespace Voxel {

#define VOXEL_SCENE_FILE_FILTER "Voxel Scene (*.voxelscene)\0*.voxelscene\0"

	class FileDialogs
	{
	public:

		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);
	};

}