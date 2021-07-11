#include <pch.h>
#include "EntityRelationship.h"

namespace Voxel {

	/*void EntityRelationshipComponent::SetParent(Entity& entity, Entity& parent)
	{
		auto& parentRelationship = parent.GetOrAddComponent<EntityRelationshipComponent>();
		auto& relationship = entity.GetOrAddComponent<EntityRelationshipComponent>();

		if (parentRelationship.FirstChild)
		{
			Entity current = parentRelationship.FirstChild;
			while(current.GetOrAddComponent<EntityRelationshipComponent>().Next)
				current = current.GetComponent<EntityRelationshipComponent>().Next;

			current.GetComponent<EntityRelationshipComponent>().Next = entity;
			relationship.Previous = current;
		} 
		else
		{
			parentRelationship.FirstChild = entity;
		}

		relationship.Parent = parent;
		parentRelationship.Children++;
	}*/

	void EntityRelationshipComponent::OnImGuiRender()
	{
	}

	Entity EntityRelationshipComponent::GetChildAt(int index)
	{
		Entity current = FirstChild;

		if (index == -1)
			index = Children - 1;

		for (size_t i = 0; i < index; i++)
			current = current.GetOrAddComponent<EntityRelationshipComponent>().Next;


		return current;
	}

	void EntityRelationshipComponent::AddChild(Entity& parent, Entity& child, int index)
	{
		auto& childRelationship = child.GetOrAddComponent<EntityRelationshipComponent>();
		auto& parentRelationship = parent.GetOrAddComponent<EntityRelationshipComponent>();

		if (parentRelationship.FirstChild)
		{
			Entity current = parentRelationship.GetChildAt(index);
			childRelationship.Next = current;
		}
		else
		{
			parentRelationship.FirstChild = child;
		}

		childRelationship.Parent = parent;
		parentRelationship.Children++;
	}

	void EntityRelationshipComponent::SetParent(Entity& entity, Entity& parent)
	{
		parent.GetOrAddComponent<EntityRelationshipComponent>().AddChild(parent, entity);
	}

}