#include <pch.h>

#include "WindowsWindow.h"

#include "Voxel/Events/ApplicationEvent.h"
#include "Voxel/Events/KeyEvent.h"
#include "Voxel/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"
//#include "Platform/Vulkan/VulkanContext.h"

namespace Voxel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Maximized = false;
		m_Data.Minimized = false;

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			CORE_ASSERT(glfwInit(), "Could not initialize GLFW");
			s_GLFWInitialized = true;

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_SAMPLES, 4);

		m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(), NULL, NULL);

		// TODO: Automatically Create Window context
		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();;

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// SET GLFW CALLBACKS

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);

		});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowFramebufferResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<KeyCode>(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 1);
					data.EventCallback(event);
					break;
				}
			default:
				break;
			}

		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(static_cast<KeyCode>(key));
			data.EventCallback(event);

		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(static_cast<MouseButton>(button));
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(static_cast<MouseButton>(button));
					data.EventCallback(event);
					break;
				}
			default:
				break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)x, (float)y);
			data.EventCallback(event);
		});

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int minimized) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Minimized = minimized;
			data.Maximized = false;

			if (minimized == GLFW_TRUE)
				data.EventCallback(WindowMinimizeEvent());
			else
				data.EventCallback(WindowRestoreEvent());
		});

		glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Maximized = maximized;
			data.Minimized = false;

			if (maximized == GLFW_TRUE)
				data.EventCallback(WindowMaximizeEvent());
			else
				data.EventCallback(WindowRestoreEvent());
		});

	}

	void WindowsWindow::Shutdown()
	{
		m_Context->Destroy();
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();

		m_Context->SwapBuffers();

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return m_Data.VSync; }

	void WindowsWindow::SetMinimized(bool minimized)
	{
		if (minimized)
			glfwIconifyWindow(m_Window);
		else
			glfwRestoreWindow(m_Window);

		m_Data.Maximized = false;
		m_Data.Minimized = minimized;
	}

	bool WindowsWindow::IsMinimized() const { return m_Data.Minimized; }

	void WindowsWindow::SetMaximized(bool maximized)
	{
		if (maximized)
			glfwMaximizeWindow(m_Window);
		else
			glfwRestoreWindow(m_Window);

		m_Data.Minimized = false;
		m_Data.Maximized = maximized;
	}

	bool WindowsWindow::IsMaximized() const { return m_Data.Maximized; }

}