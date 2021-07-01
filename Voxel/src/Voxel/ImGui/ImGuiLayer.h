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

		void BlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }
		bool IsBlockingEvents() { return m_BlockEvents; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0;
	};

}