#pragma once

#include "HorizonEngine/Window.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/LayerStack.h"
#include "HorizonEngine/FileManagement/ProjectFile.h"
#include "HorizonEngine/Input.h"
#include "HorizonEngine/ImGui/ImGuiLayer.h"
#include "HorizonEngine/Renderer/Shader.h"

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
		static App& getApp() { return *m_Instance; }
		Window& getAppWindow() { return *m_AppWindow; }

	private:
		static App* m_Instance;
		bool m_Running;

		std::unique_ptr<Window> m_AppWindow;
		std::unique_ptr<Input> m_Input;
		std::unique_ptr<Shader> m_Shader;

		ImguiLayer* m_ImguiLayer;
		LayerStack m_Layers;
		
		unsigned int m_VertexArray = 0;
		unsigned int m_VertexBuffer = 0;
		unsigned int m_ElementBuffer = 0;
	};

	// to be defined by the application that implements this function
	std::shared_ptr<App> createApp();
}
