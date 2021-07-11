#pragma once

#include "Voxel.h"

#include "Voxel/Scene/Scene.h"
#include "Voxel/Scene/Entity.h"

namespace Voxel {

	struct EntityRelationshipComponent
	{
		size_t Children = 0;
		Entity FirstChild = {};

		Entity Next = {};
		Entity Parent = {};

		EntityRelationshipComponent() = default;
		void OnImGuiRender();

		static void AddChild(Entity& parent, Entity& child, int index = -1);
		static void SetParent(Entity& entity, Entity& parent);

		Entity GetChildAt(int index);
		Entity operator[](size_t index) { return GetChildAt((int)index); }
	};

}