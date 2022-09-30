#include "pch.h"
#include "App.h"
#include "Window.h"
#include "platform/windows/MSWindow.h"
#include "HorizonEngine/Events/MouseEvent.h"

namespace Hzn
{
	void App::run()
	{
		MouseMovedEvent m(1, 2);
		std::cout << m.ToString() << std::endl;
		std::cout << "Welcome to HorizonEngine!" << std::endl;
		Window* w = new MSWindow(1366, 768, "HorizonEngine");
		while (!w->shouldClose())
		{
			w->update();
			w->render();
		}
		delete w;
	}
}