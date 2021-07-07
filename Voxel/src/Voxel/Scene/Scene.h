#pragma once

#include "Voxel/Core/Timestep.h"
#include "Voxel/Rendering/EditorCamera.h"

#include "entt.hpp"

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

		static Ref<Scene>& Scene::LoadScene(const std::string& path);
		static Ref<Scene>& Scene::SampleScene();
	private:
		entt::registry m_Registry;
		std::string m_Name;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}