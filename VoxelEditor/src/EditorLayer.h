#pragma once

#include "Voxel.h"

#include <ImGui/imgui.h>

#include "Panels/Panel.h"
#include "EditorState.h"

#include "Voxel/Rendering/EditorCamera.h"

namespace Voxel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(Timestep & timestep);
		virtual void OnEvent(Event & event);

		virtual void OnImGuiRender();
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnWindowRestored(WindowRestoreEvent& event);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		EditorState m_EditorState;
		std::vector<Ref<Panel>> m_Panels;
	};

}