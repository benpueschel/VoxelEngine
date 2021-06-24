#include <pch.h>
#include "Application.h"
#include "Time.h"

#include "Voxel/Rendering/Renderer.h"

#include <GLFW/glfw3.h>

namespace Voxel {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(!s_Instance, "Singleton-class Application already exists");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
		PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowFramebufferResizeEvent>(BIND_EVENT_FN(Application::OnWindowFramebufferResize));

		//LOG_CORE_DEBUG("{0}", event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		PROFILE_FUNCTION();
		m_Running = false;
		return true;
	}

	bool Application::OnWindowFramebufferResize(WindowFramebufferResizeEvent& event)
	{
		PROFILE_FUNCTION();
		if (m_Window->IsMinimized())
			return false;

		Renderer::OnWindowFramebufferResize(event);
		return false;
	}

	void Application::Run()
	{
		PROFILE_FUNCTION();

		while (m_Running)
		{
			PROFILE_SCOPE("Application RunLoop");

			float time = (float)glfwGetTime();
			float deltaTime = time - m_LastFrameTime;
			m_Timestep = { time, deltaTime };

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			if (!m_Window->IsMinimized())
			{
				{
					PROFILE_SCOPE("Layer Updating");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(m_Timestep);
				}

				{
					PROFILE_SCOPE("ImGui Rendering");

					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}
			}

			m_Window->OnUpdate();

			m_LastFrameTime = time;
		}
	}

}