#pragma once

#include "Voxel/Core/Timestep.h"

#include "entt.hpp"

namespace Voxel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Empty Entity");
		void DestroyEntity(Entity& entity);

		std::vector<Entity*> GetEntities();

		void OnUpdate(Timestep timestep);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}