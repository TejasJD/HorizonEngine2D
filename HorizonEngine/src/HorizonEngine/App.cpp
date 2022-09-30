#include "App.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "box2d/box2d.h"

/*
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
*/

#include "HorizonEngine/Events/MouseEvent.h"

namespace Hzn
{
	void App::run()
	{
		MouseMovedEvent a(5.55, 7.02);
		a.ToString();
		std::cout << a.ToString() << std::endl;

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