#pragma once

#include "Panel.h"
#include "../EditorState.h"

#include "SceneHierarchyPanel.h"
#include "Voxel/Scene/Entity.h"

#include <imgui.h>

namespace Voxel {

	class PropertiesPanel : public Panel
	{
	public:
		PropertiesPanel(EditorState& state)
			: m_State(state), Panel("Properties") { }

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Timestep& timestep) override;
		virtual void OnEvent(Event& event) override;

		void Lock(bool locked) { m_Locked = locked; }

	private:
		//TODO: Move "removable" to the component
		template<typename Component>
		void DrawComponent(const char* name, int treeNodeFlags, bool removable = true);
	private:
		bool m_Locked = false;
		Entity m_Context;
		EditorState& m_State;
	};

}