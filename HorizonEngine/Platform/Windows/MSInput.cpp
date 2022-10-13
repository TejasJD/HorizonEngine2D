#include "pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "HorizonEngine/App.h"
#include "MSWindow.h"
#include "MSInput.h"

namespace Hzn
{
	Input* Input::m_Instance = nullptr;
	Input* Input::createInstance()
	{ 
		if (!m_Instance)
		{
			m_Instance = new MSInput();
		}

		return m_Instance;
	}


	bool MSInput::keyPressedUtil(int key) const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		
		int action = glfwGetKey(window, key);
		return action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	bool MSInput::mouseButtonPressedUtil(int button) const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		int action = glfwGetMouseButton(window, button);
		return action == GLFW_PRESS;
	}

	std::pair<double, double> MSInput::getMousePosUtil() const
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		std::pair<double, double> pos;
		glfwGetCursorPos(window, &pos.first, &pos.second);
		return pos;
	}

	double MSInput::getMouseXUtil() const
	{
		return getMousePosUtil().first;
	}

	double MSInput::getMouseYUtil() const
	{
		return getMousePosUtil().second;
	}
}