#include "pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "HorizonEngine/App.h"
#include "GLWindow.h"
#include "HorizonEngine/Input.h"

namespace Hzn
{
	bool Input::keyPressed(int key)
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		
		int action = glfwGetKey(window, key);
		return action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	bool Input::mouseButtonPressed(int button)
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		int action = glfwGetMouseButton(window, button);
		return action == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePos()
	{
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		std::pair<double, double> pos;
		glfwGetCursorPos(window, &pos.first, &pos.second);
		return pos;
	}

	float Input::getMouseX()
	{
		return getMousePos().first;
	}

	float Input::getMouseY()
	{
		return getMousePos().second;
	}
}