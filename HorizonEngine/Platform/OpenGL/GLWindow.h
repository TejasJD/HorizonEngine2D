#pragma once

#include "HorizonEngine/Window.h"
#include "HorizonEngine/Renderer/RenderContext.h"
// Implementation of the Window class with OpenGL as the rendering API

struct GLFWwindow;

namespace Hzn
{

	class GLWindow : public Window
	{
	public:
		// Constructor creates window and initializes the object
		GLWindow(const unsigned int& width, const unsigned int& height, const char* const& title);
		~GLWindow();

		// Inherited via Window
		virtual void onUpdate() override;
		virtual unsigned int getHeight() override { return m_Data.height; };
		virtual unsigned int getWidth() override { return m_Data.width; }
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_Data.callback = callback; }
		virtual void* getPlatformRawWindow() { return m_Window; }
		virtual void setVsync(bool vsync) override { m_Vsync = vsync; }
		virtual bool getVsync() const override { return m_Vsync;  }

	private:
		void init();
		void destroy();

		struct WindowData
		{
			unsigned int width;
			unsigned int height;
			const char* title;
			EventCallbackFn callback;
		};

		WindowData m_Data;
		GLFWwindow* m_Window;
		bool m_Vsync = false;

		std::unique_ptr<RenderContext> m_Context;
	};
}