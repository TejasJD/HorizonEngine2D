#pragma once

#include "imgui.h"

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Window.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/LayerStack.h"
#include "HorizonEngine/Input.h"
#include "HorizonEngine/ImGui/ImGuiLayer.h"

namespace Hzn
{
	class App
	{
	public:
		App();

		virtual ~App() {}

		void addLayer(Layer* layer) 
		{
			m_Layers.addLayer(layer);
			layer->onAttach();
		}
		void addOverlay(Layer* layer)
		{ 
			m_Layers.addOverlay(layer);
			layer->onAttach();
		}

		void run();
		bool onWindowClose(WindowCloseEvent& e);
		void onEvent(Event& e);

		ImGuiContext* getImGuiAppContext() { return g; }
		
		static App& getApp() { return *m_Instance; }
		Window& getAppWindow() { return *m_AppWindow; }
	protected:
	private:
		ImGuiContext* g;
		std::unique_ptr<Window> m_AppWindow;
		std::unique_ptr<Input> m_Input;
		// UI layer provided by the game engine to the client application
		ImguiLayer* m_ImguiLayer;
		bool m_Running;
		LayerStack m_Layers;
		
		static App* m_Instance;
	};

	// to be defined by the application that implements this function
	std::shared_ptr<App> createApp();
}
