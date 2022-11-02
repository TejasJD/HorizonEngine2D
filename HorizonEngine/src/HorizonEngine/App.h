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
#include "HorizonEngine/Renderer/Buffer.h"
#include "HorizonEngine/Renderer/VertexArray.h"
#include "HorizonEngine/Renderer/Renderer.h"
#include "HorizonEngine/Core/TimeStep.h"

#include "HorizonEngine/Components/ComponentFactory.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Components/Transform.h"
#include "HorizonEngine/Physics2D/BoxCollider2D.h"
#include "HorizonEngine/Physics2D/Rigidbody2D.h"

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


		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		static App& getApp() { return *m_Instance; }
		Window& getAppWindow() { return *m_Window; }
		void registerComponents();

	protected:
		static App* m_Instance;

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		std::shared_ptr<Window> m_Window;
		std::shared_ptr<Input> m_Input;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<ElementBuffer> m_ElementBuffer;

		ImguiLayer* m_ImguiLayer;
		LayerStack m_Layers;
		TimeStep m_TimeStep;
		float lastFrameTime = 0.0f;
	};

	// to be defined by the application that implements this function
	std::shared_ptr<App> createApp();
}
