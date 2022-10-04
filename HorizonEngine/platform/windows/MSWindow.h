#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Window.h"
// Implementation of the Window class with OpenGL as the rendering API
namespace Hzn
{

	class HZN_API MSWindow : public Window
	{
	public:
		// Constructor creates window and initializes the object
		MSWindow(const unsigned int& width, const unsigned int& height, const char* const& title);
		~MSWindow();

		// Inherited via Window
		virtual void onUpdate() override;
		virtual unsigned int getHeight() override { return m_Data.height; };
		virtual unsigned int getWidth() override { return m_Data.width; }
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_Data.callback = callback; }

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
	};
}