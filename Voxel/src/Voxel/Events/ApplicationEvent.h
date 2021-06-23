#pragma once

#include "Event.h"

namespace Voxel {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{
		}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowFramebufferResizeEvent : public Event
	{
	public:
		WindowFramebufferResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{
		}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowFramebufferResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowFramebufferResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent() = default;

		EVENT_CLASS_TYPE(WindowMinimized)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent() = default;

		EVENT_CLASS_TYPE(WindowMaximized)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowRestoreEvent : public Event
	{
	public:
		WindowRestoreEvent() = default;

		EVENT_CLASS_TYPE(WindowRestored)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}