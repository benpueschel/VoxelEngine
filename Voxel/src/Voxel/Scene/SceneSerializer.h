#pragma once

#include "Scene.h"
#include <filesystem>

namespace Voxel {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeText(const std::filesystem::path& path);
		void SerializeBinary(const std::filesystem::path& path);

		bool DeserializeText(const std::filesystem::path& path);
		bool DeserializeBinary(const std::filesystem::path& path);
	private:
		Ref<Scene> m_Scene;
	};

}