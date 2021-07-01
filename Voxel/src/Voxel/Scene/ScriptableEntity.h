#pragma once

#include "Entity.h"
#include "Voxel/Core/Timestep.h"

namespace Voxel {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) { return m_Entity.AddComponent<T>(args); }

		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }

		template<typename T>
		bool HasComponent() { return m_Entity.HasComponent<T>(); }

		template<typename T>
		void RemoveComponent() { m_Entity.RemoveComponent(); }

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};

}