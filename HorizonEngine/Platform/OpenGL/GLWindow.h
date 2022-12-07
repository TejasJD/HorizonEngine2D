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
		GLWindow(const unsigned int& width, const unsigned int& height, const char* const& title, bool maximized);
		~GLWindow();

		// Inherited via Window
		virtual void onUpdate() override;
		virtual unsigned int getHeight() override { return m_Data.height; };
		virtual unsigned int getWidth() override { return m_Data.width; }
		virtual float getAspectRatio() override { return (float)m_Data.width / (float)m_Data.height; }
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_Data.callback = callback; }
		virtual void* getPlatformRawWindow() { return m_Window; }
		virtual void setVsync(bool vsync) override { m_Vsync = vsync; }
		virtual bool getVsync() const override { return m_Vsync;  }
		virtual void setCursorMode(CursorMode mode) override;

	private:
		void init();
		void destroy();

		struct WindowData
		{
			uint32_t width;
			uint32_t height;
			const char* title;
			EventCallbackFn callback;
		};

		WindowData m_Data;
		GLFWwindow* m_Window;
		bool m_Vsync = false;
		bool m_Maximized = false;

		std::shared_ptr<RenderContext> m_Context;
	};
}