#pragma once

#include "Panel.h"
#include "../EditorState.h"

#include "Voxel.h"

namespace Voxel {

	struct PropertiesPanel;

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(EditorState& state) 
			: m_State(state), Panel("Scene Hierarchy") {}

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void DrawEntityNode(Entity& entity);
	private:
		EditorState& m_State;
	};

}
