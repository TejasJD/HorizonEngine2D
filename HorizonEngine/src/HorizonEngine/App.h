#pragma once

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Window.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/LayerStack.h"

namespace Hzn
{
	class HZN_API App
	{
	public:
		App();

		virtual ~App() {}

		void addLayer(Layer* layer) { m_Layers.addLayer(layer); }
		void addOverlay(Layer* layer) { m_Layers.addOverlay(layer); }

		void run();
		bool onWindowClose(WindowCloseEvent& e);
		void onEvent(Event& e);

	private:
		std::unique_ptr<Window> m_AppWindow;
		bool m_Running;
		LayerStack m_Layers;
	};

	// to be defined by the application that implements this function
	HZN_API std::shared_ptr<App> createApp();
}
