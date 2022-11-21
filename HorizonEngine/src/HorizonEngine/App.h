#pragma once

#include "HorizonEngine/Window.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/LayerStack.h"
#include "HorizonEngine/FileManagement/ProjectFile.h"
#include "HorizonEngine/ImGui/ImGuiLayer.h"
#include "HorizonEngine/Core/TimeStep.h"

namespace Hzn
{
	class App
	{
	public:
		App();
		virtual ~App();

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
		void onEvent(Event& e);
		void close() { m_Running = false; }

		Layer* getImguiLayer() { return m_ImguiLayer; }

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		static App& getApp() { return *m_Instance; }
		Window& getAppWindow() const { return *m_Window; }

	protected:
		static App* m_Instance;

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		std::shared_ptr<Window> m_Window;

		ImguiLayer* m_ImguiLayer;
		LayerStack m_Layers;
		TimeStep m_TimeStep;
		float lastFrameTime = 0.0f;
	};

	// to be defined by the application that implements this function
	std::shared_ptr<App> createApp();
}
