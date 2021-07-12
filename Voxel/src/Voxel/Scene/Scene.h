#pragma once

#include "Voxel/Core/Timestep.h"
#include "Voxel/Rendering/EditorCamera.h"

#include "entt.hpp"

#include <filesystem>

namespace Voxel {

	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name = "Unnamed Scene");
		~Scene();

		Entity CreateEntity(const std::string& name = "Empty Entity");
		void DestroyEntity(Entity& entity);

		void OnUpdateRuntime(Timestep timestep);
		void OnUpdateEditor(Timestep timestep, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera();

		void SetName(const std::string& name);
		std::string& GetName();

		void SetSavePath(const std::filesystem::path& savePath) { m_SavePath = savePath; }
		std::filesystem::path& GetSavePath() { return m_SavePath; }

		static Ref<Scene>& LoadScene(const std::string& path);
		static Ref<Scene>& SampleScene();
	private:
		entt::registry m_Registry;
		std::string m_Name;
		std::filesystem::path m_SavePath = std::filesystem::path();
		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}