#pragma once

#include "Voxel/Core/Core.h"

#include "Scene.h"
#include "entt.hpp"

namespace Voxel {

	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;

		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity already owns this component");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not own this component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetOrAddComponent()
		{
			if (HasComponent<T>())
				return GetComponent<T>();
			return AddComponent<T>();
		}

		void GetComponents()
		{
			m_Scene->m_Registry.visit(m_EntityHandle, [&](const entt::type_info info) {
				auto& component = m_Scene->m_Registry.get(m_EntityHandle);
				
			});
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not own this component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		Scene* GetScene()
		{
			return m_Scene;
		}

		bool operator ==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator !=(const Entity& other) const { return !(*this == other); }

		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator uint32_t() { return (uint32_t)m_EntityHandle; }
		operator entt::entity() { return m_EntityHandle; }

		operator bool() { return m_EntityHandle != entt::null && m_Scene; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}
