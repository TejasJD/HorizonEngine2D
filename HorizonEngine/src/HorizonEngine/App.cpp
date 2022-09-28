#include "App.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "box2d/box2d.h"

namespace Hzn
{
	void App::run()
	{
		if (!glfwInit())
		{
			std::cout << "GLFW couldn't be initialized!" << std::endl;
			return;
		}
		std::cout << "GLFW initialized!" << std::endl;

		b2Body* body;
		std::cout << "Welcome to Horizon Engine Application!" << std::endl;
		while (true);
	}
}