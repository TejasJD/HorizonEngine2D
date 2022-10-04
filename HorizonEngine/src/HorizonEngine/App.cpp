#include "pch.h"
#include "App.h"

namespace Hzn
{
	App* App::m_Instance = nullptr;
	//! App class constructor, initializes the application
	App::App(): m_Running(true)
	{
		assert(m_Instance == nullptr, "application already initialized");
		m_Instance = this;
		m_AppWindow = std::unique_ptr<Window>(Window::create());
		m_AppWindow->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));
	}

	//! the main App run loop. This loop keeps the application running and updates and renders
	//! different layers
	void App::run()
	{
		while (m_Running)
		{
			// we go through all the layers
			// and update each of them
			for (auto& layer : m_Layers)
			{
				layer->onUpdate();
			}
			m_AppWindow->onUpdate();
		}
	}

	//! terminates the App run loop
	//! always returns true
	bool App::onWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	//! the onEvent function of application class that handles any events coming to the application
	void App::onEvent(Event& e)
	{
		// the event dispatcher will take in our Event e
		// here, the dispatcher now dispatches the event to a
		// different function that might need to handle such event
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::onWindowClose, this, std::placeholders::_1));
		HZN_CORE_TRACE(e);

		//! update the added layers in the application, in reverse order.
		//! this means we would go through all the overlays and then the layers.
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			//! we go through all the layers and check if the event has been handled
			//! if yes, we break
			(*it)->onEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}
}