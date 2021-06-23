#pragma once

#include "Voxel/Core/Window.h"
#include "Voxel/Rendering/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Voxel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetMinimized(bool minimized) override;
		bool IsMinimized() const override;
		void SetMaximized(bool maximized) override;
		bool IsMaximized() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool Minimized;
			bool Maximized;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}