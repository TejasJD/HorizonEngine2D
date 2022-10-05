#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	class HZN_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void onUpdate() = 0;
		virtual unsigned int getHeight() = 0;
		virtual unsigned int getWidth() = 0;
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;

		//! returns a void pointer which can be casted to concrete window, based on the active platform.
		virtual void* getPlatformRawWindow() = 0;
		
		//! Window creation implemented by the source files based on the active platform
		static Window* createInstance(const unsigned int&
			width = 1366, const unsigned int& height = 768, const char *const& title = "Window");
	};
}