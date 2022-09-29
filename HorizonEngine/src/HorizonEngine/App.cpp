#include "pch.h"
#include "App.h"
#include "Window.h"
#include "platform/windows/MSWindow.h"

namespace Hzn
{
	void App::run()
	{
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