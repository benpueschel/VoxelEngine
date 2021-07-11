#include <pch.h>
#include "EntityRelationship.h"

namespace Voxel {
	
	void EntityRelationshipComponent::SetParent(Entity& entity, Entity& parent)
	{
		auto& parentRelationship = parent.GetOrAddComponent<EntityRelationshipComponent>();
		auto& relationship = entity.GetOrAddComponent<EntityRelationshipComponent>();

		if (parentRelationship.FirstChild)
		{
			Entity& current = parentRelationship.FirstChild;
			for (size_t i = 0; i < parentRelationship.Children - 1; i++)
				current = current.GetOrAddComponent<EntityRelationshipComponent>().Next;

			current.GetOrAddComponent<EntityRelationshipComponent>().Next = entity;
			relationship.Previous = current;
		} 
		else
		{
			parentRelationship.FirstChild = entity;
		}

		relationship.Parent = parent;
		parentRelationship.Children++;
	}

	void EntityRelationshipComponent::OnImGuiRender()
	{
	}

}