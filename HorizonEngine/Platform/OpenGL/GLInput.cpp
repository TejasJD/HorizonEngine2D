#include "pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "HorizonEngine/App.h"
#include "GLWindow.h"
#include "GLInput.h"

namespace Hzn
{
	std::shared_ptr<Input> Input::m_Instance;
	std::shared_ptr<Input> Input::create()
	{ 
		if (!m_Instance)
		{
			m_Instance = std::make_shared<GLInput>();
		}

		return m_Instance;
	}


	bool GLInput::keyPressedUtil(int key) const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		
		int action = glfwGetKey(window, key);
		return action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	bool GLInput::mouseButtonPressedUtil(int button) const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		int action = glfwGetMouseButton(window, button);
		return action == GLFW_PRESS;
	}

	std::pair<double, double> GLInput::getMousePosUtil() const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		std::pair<double, double> pos;
		glfwGetCursorPos(window, &pos.first, &pos.second);
		return pos;
	}

	double GLInput::getMouseXUtil() const
	{
		return getMousePosUtil().first;
	}

	double GLInput::getMouseYUtil() const
	{
		return getMousePosUtil().second;
	}
}