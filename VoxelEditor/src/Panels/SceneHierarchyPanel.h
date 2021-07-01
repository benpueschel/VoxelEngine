#pragma once

#include "Voxel.h"

#include "PropertiesPanel.h"

namespace Voxel {

	struct PropertiesPanel;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetPropertiesPanel(const Ref<PropertiesPanel>& panel);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity& entity);
	private:
		Ref<Scene> m_Context;
		Ref<PropertiesPanel> m_Properties;
	};

}
