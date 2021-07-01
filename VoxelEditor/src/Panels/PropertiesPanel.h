#pragma once

#include "SceneHierarchyPanel.h"
#include "Voxel/Scene/Entity.h"

#include <imgui.h>

namespace Voxel {

	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(Entity& context)
			: m_Context(context) { }

		void SetContext(Entity& context)
		{
			m_Context = context;
		}

		void OnImGuiRender();
	private:
		//TODO: Move "removable" to the component
		template<typename Component>
		void DrawComponent(const char* name, int treeNodeFlags, bool removable = true);
	private:
		Entity m_Context;

		friend class SceneHierarchyPanel;
	};

}