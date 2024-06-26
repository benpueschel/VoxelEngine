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

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CORE_ASSERT((index < Count), "Index out of bounds");
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Voxel Engine", ApplicationCommandLineArgs args = {});
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime;

		ApplicationCommandLineArgs m_CommandLineArgs;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);

}