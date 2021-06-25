#pragma once

#include <ImGui/imgui.h>

namespace Voxel {

	class EditorViewport 
	{
	public:
		void OnImGuIRender();

	private:
		ImVec2 m_ViewportSize;
	};

}

