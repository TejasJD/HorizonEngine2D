#include "pch.h"

#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "GLContext.h"

#include "MSWindow.h"

namespace Hzn
{
	void errorCallback (int error_code, const char* description)
	{
		HZN_CORE_CRITICAL("{0}: {1}", error_code, description);
	}
	//! implements window creation for windows platform
	HZN_API Window* Window::createInstance(const unsigned int& width, const unsigned int& height, const char* const& title)
	{
		return new MSWindow(width, height, title);
	}

	MSWindow::MSWindow(const unsigned int& width, const unsigned int& height, const char* const& title)
		: m_Data{ width, height, title }
	{
		init();
	}

	void MSWindow::onUpdate()
	{
		glfwPollEvents();
		m_Context->swapBuffers();
	}

	MSWindow::~MSWindow()
	{
		destroy();
	}

	void MSWindow::init()
	{
		int glfwSuccess = glfwInit();
		HZN_CORE_ASSERT(glfwSuccess, "Failed to initialize GLFW!");

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, nullptr, nullptr);
		m_Context = new GLContext(m_Window);
		m_Context->init();

		//! set the glfwWindowUserPointer to hold additional data.
		//! This data holds the dimensions, the title and the callback function as well.
		glfwSetWindowUserPointer(m_Window, &m_Data);

		//! sets callback to the window close event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent eventObj;
				data.callback(eventObj);
			});

		//! sets event to resize the window
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent eventObj(width, height);
				data.callback(eventObj);
				glViewport(0, 0, width, height);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent eventObj(x, y);
				data.callback(eventObj);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent eventObj(x, y);
				data.callback(eventObj);

			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

				if (action == GLFW_PRESS)
				{
					MouseButtonPressedEvent eventObj(button);
					data.callback(eventObj);
				}
				else
				{
					MouseButtonReleasedEvent eventObj(button);
					data.callback(eventObj);
				}
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_PRESS)
				{
					KeyPressedEvent k(key, 0);
					data.callback(k);
				}
				else if (action == GLFW_REPEAT)
				{
					KeyPressedEvent k(key, 1);
					data.callback(k);
				}
				else
				{
					KeyReleasedEvent k(key);
					data.callback(k);
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(codepoint);
				data.callback(event);
			});

		glfwSetErrorCallback(errorCallback);
	}

	void MSWindow::destroy()
	{
		glfwDestroyWindow(m_Window);
	}
}
