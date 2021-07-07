#pragma once

#include <pch.h>

#include "Voxel/Core/Core.h"
#include "Voxel/Events/Event.h"

namespace Voxel {

	enum class WindowMode : uint32_t
	{
		WINDOWED = 0,
		FULLSCREEN = 1,
		BORDERLESS_WINDOW = 2,
		BORDERLESS_FULLSCREEN = 3
	};

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		WindowMode Mode;

		WindowProps(const std::string& title = "Voxel Engine", uint32_t width = 1600, uint32_t height = 900, 
			WindowMode mode = WindowMode::WINDOWED) 
			: Title(title), Width(width), Height(height), Mode(mode)
		{
		}
	};

	// system-independent window interface
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetWindowMode(WindowMode windowMode) = 0;
		virtual WindowMode GetWindowMode() = 0;

		virtual void SetMinimized(bool minimized) = 0;
		virtual bool IsMinimized() const = 0;

		virtual void SetMaximized(bool maximized) = 0;
		virtual bool IsMaximized() const = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual std::string& GetTitle() = 0;


		virtual void* GetNativeWindow() const = 0;

		static Scope <Window> Create(const WindowProps &props = WindowProps());
	};

}