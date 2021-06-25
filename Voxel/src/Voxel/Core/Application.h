#pragma once

#include "Core.h"
#include "Window.h"

#include "Voxel/Core/LayerStack.h"
#include "Voxel/Events/Event.h"
#include "Voxel/Events/ApplicationEvent.h"

#include "Voxel/ImGui/ImGuiLayer.h"

#include "Voxel/Rendering/Shaders/Shader.h"
#include "Voxel/Rendering/Shaders/Buffer.h"
#include "Voxel/Rendering/Shaders/VertexArray.h"

#include "Timestep.h"

namespace Voxel {

	class Application
	{
	public:
		Application(const std::string& name = "Voxel Engine");
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowFramebufferResize(WindowFramebufferResizeEvent& event);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}