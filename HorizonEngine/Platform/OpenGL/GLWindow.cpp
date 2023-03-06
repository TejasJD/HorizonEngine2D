#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"

#include "GLContext.h"
#include "GLWindow.h"

namespace Hzn
{
	void errorCallback (int error_code, const char* description)
	{
		HZN_CORE_CRITICAL("{0}: {1}", error_code, description);
	}

	GLWindow::GLWindow(const unsigned int& width, const unsigned int& height, const char* const& title, bool maximized)
		: m_Data{ width, height, title }, m_Maximized(maximized)
	{
		init();
	}

	void GLWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapInterval(getVsync() ? 1 : 0);
		m_Context->swapBuffers();
	}

	GLWindow::~GLWindow()
	{
		destroy();
	}

	void GLWindow::init()
	{
		int glfwSuccess = glfwInit();
		HZN_CORE_ASSERT(glfwSuccess, "Failed to initialize GLFW!");

		if(m_Maximized) glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, nullptr, nullptr);
		m_Context = RenderContext::create(m_Window);
		m_Context->init();

		//! set the glfwWindowUserPointer to hold additional data.
		//! This data holds the dimensions, the title and the callback function as well.
		glfwSetWindowUserPointer(m_Window, &m_Data);

		//! sets callback to the window close event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent eventObj;
				data->callback(eventObj);
			});

		//! sets event to resize the window
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->width = width;
				data->height = height;
				
				WindowResizeEvent eventObj(width, height);
				data->callback(eventObj);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent eventObj(static_cast<float>(x), static_cast<float>(y));
				data->callback(eventObj);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent eventObj(static_cast<float>(x), static_cast<float>(y));
				data->callback(eventObj);

			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

				if (action == GLFW_PRESS)
				{
					MouseButtonPressedEvent eventObj(button);
					data->callback(eventObj);
				}
				else
				{
					MouseButtonReleasedEvent eventObj(button);
					data->callback(eventObj);
				}
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_PRESS)
				{
					KeyPressedEvent k(key, 0);
					data->callback(k);
				}
				else if (action == GLFW_REPEAT)
				{
					KeyPressedEvent k(key, 1);
					data->callback(k);
				}
				else
				{
					KeyReleasedEvent k(static_cast<KeyCode>(key));
					data->callback(k);
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(codepoint);
				data->callback(event);
			});

		glfwSetErrorCallback(errorCallback);
	}

	void GLWindow::setCursorMode(CursorMode mode)
	{
		switch (mode)
		{
		case Hzn::CursorMode::Enabled:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			return;
		case Hzn::CursorMode::Disabled:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			return;
		case Hzn::CursorMode::Hidden:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			return;
		}
		HZN_CORE_ASSERT(false, "Unknown cursor mode!");
	}

	void GLWindow::destroy()
	{
		glfwDestroyWindow(m_Window);
	}
}
