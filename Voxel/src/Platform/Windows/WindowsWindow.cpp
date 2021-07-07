#include <pch.h>

#include "WindowsWindow.h"

#include "Voxel/Events/ApplicationEvent.h"
#include "Voxel/Events/KeyEvent.h"
#include "Voxel/Events/MouseEvent.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"
//#include "Platform/Vulkan/VulkanContext.h"

namespace Voxel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Maximized = false;
		m_Data.Minimized = false;

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			PROFILE_SCOPE("glfwInit");
			CORE_ASSERT(glfwInit(), "Could not initialize GLFW");
			s_GLFWInitialized = true;

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			PROFILE_SCOPE("glfwCreateWindow");		
			
			glfwWindowHint(GLFW_SAMPLES, 4);

		#ifdef VOXEL_DEBUG
			if (Renderer::GetAPI() == RenderAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

			m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}
		// TODO: Automatically Create Window context
		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();;

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);
		SetMaximized(true);

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

			int width, height;
			glfwGetWindowSize(window, &width, &height);

			data.Width = width;
			data.Height = height;

			if (minimized == GLFW_TRUE)
				data.EventCallback(WindowMinimizeEvent());
			else
				data.EventCallback(WindowRestoreEvent(data.Width, data.Height));
		});

		glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Maximized = maximized;
			data.Minimized = false;

			int width, height;
			glfwGetWindowSize(window, &width, &height);

			data.Width = width;
			data.Height = height;

			if (maximized == GLFW_TRUE)
				data.EventCallback(WindowMaximizeEvent(data.Width, data.Height));
			else
				data.EventCallback(WindowRestoreEvent(data.Width, data.Height));
		});

	}

	void WindowsWindow::Shutdown()
	{
		PROFILE_FUNCTION();

		m_Context->Destroy();
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		PROFILE_FUNCTION();

		glfwPollEvents();

		m_Context->SwapBuffers();

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		PROFILE_FUNCTION();

		glfwSwapInterval(enabled ? 1 : 0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return m_Data.VSync; }


	void WindowsWindow::SetWindowMode(WindowMode windowMode)
	{
		m_Data.WindowMode = windowMode;

		// TODO: OS-independent Monitor Interface to allow other monitors
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		switch (windowMode)
		{
			case WindowMode::FULLSCREEN:
				glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				break;
			case WindowMode::BORDERLESS_FULLSCREEN:
				glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
				break;
			case WindowMode::BORDERLESS_WINDOW:
				glfwSetWindowMonitor(m_Window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
				glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
				break;
			case WindowMode::WINDOWED:
				glfwSetWindowMonitor(m_Window, nullptr, 0, 0, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
				glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
				break;
		}

	}


	Voxel::WindowMode WindowsWindow::GetWindowMode()
	{
		return m_Data.WindowMode;
	}

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

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	std::string& WindowsWindow::GetTitle()
	{
		return m_Data.Title;
	}

}