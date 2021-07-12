#pragma once

#include "Voxel/Core/Layer.h"

#include "Voxel/Events/MouseEvent.h"
#include "Voxel/Events/KeyEvent.h"
#include "Voxel/Events/ApplicationEvent.h"

namespace Voxel {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

	private:
		float m_Time = 0;
	};

}