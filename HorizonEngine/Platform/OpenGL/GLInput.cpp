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
		double x = 0.0, y = 0.0;
		glfwGetCursorPos(window, &x, &y);
		return {static_cast<float>(x), static_cast<float>(y)};
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