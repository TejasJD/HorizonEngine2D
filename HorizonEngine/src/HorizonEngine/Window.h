#pragma once

#include "HorizonEngine/Input.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void onUpdate() = 0;
		virtual uint32_t getHeight() = 0;
		virtual uint32_t getWidth() = 0;
		virtual float getAspectRatio() = 0;
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVsync(bool vsync) = 0;
		virtual bool getVsync() const = 0;
		virtual void setCursorMode(CursorMode mode) = 0;

		//! returns a void pointer which can be casted to concrete window, based on the active platform.
		virtual void* getPlatformRawWindow() = 0;
		
		//! Window creation implemented by the source files based on the active platform
		static std::shared_ptr<Window> create(
			uint32_t width = 1366,
			uint32_t height = 768,
			const char* title = "Window",
			bool maximized = false
		);
	};
}