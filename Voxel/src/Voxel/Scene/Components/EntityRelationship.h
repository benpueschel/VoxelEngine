#pragma once

#include "Voxel.h"

#include "Voxel/Scene/Scene.h"
#include "Voxel/Scene/Entity.h"

namespace Voxel {

	struct EntityRelationshipComponent
	{
		size_t Children = 0;
		Entity FirstChild = {};

		Entity Previous = {};
		Entity Next = {};
		Entity Parent = {};

		EntityRelationshipComponent() = default;

		void SetParent(Entity& entity, Entity& parent);
		void OnImGuiRender();
	};

}