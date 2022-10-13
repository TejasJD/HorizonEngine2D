#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"



#include "HorizonEngine/GameObject.h"
#include "HorizonEngine/Components/ComponentFactory.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Components/Transform.h"
#include "HorizonEngine/Physics2D/BoxCollider2D.h"
#include "HorizonEngine/Physics2D/Rigidbody2D.h"
#include "HorizonEngine/SceneManagement/Scene.h"



#include "App.h"
namespace Hzn
{

	App* App::m_Instance = nullptr;
	//! App class constructor, initializes the application
	App::App() : m_Running(true)
	{
		/*HZN_CORE_ASSERT(false, "application already initialized");*/
		m_Instance = this;
		m_Input = std::unique_ptr<Input>(Input::createInstance());
		m_AppWindow = std::unique_ptr<Window>(Window::createInstance());
		m_AppWindow->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));
		m_ImguiLayer = new ImguiLayer();
		addLayer(m_ImguiLayer);
		g = ImGui::GetCurrentContext();
	}

	//! the main App run loop. This loop keeps the application running and updates and renders
	//! different layers
	void App::run()
	{
		// Component Factory test
		REGISTER(Component, Transform);
		REGISTER(Component, BoxCollider2D);
		//std::shared_ptr<Component> t(FACTORY(Component).create("Transform"));
		// std::shared_ptr<Transform> t2 = std::dynamic_pointer_cast<Transform>(t);

		ProjectFile* p = new ProjectFile("input.txt");

		Scene* s = new Scene();
		s->open();
		std::cout << "Found objects: " << s->getObjects()->size() << std::endl;
		s->save();



		for (auto layers : m_Layers)
		{
			std::cout << layers->getName() << std::endl;
		}

		while (m_Running)
		{
			// we go through all the layers
			// and update each of them
			/*m_AppWindow->onUpdate();*/
			glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto& layer : m_Layers)
			{
				layer->onUpdate();
			}


			m_ImguiLayer->imguiBegin();
			for (auto& layer : m_Layers)
			{
				layer->onRenderImgui();
			}
			m_ImguiLayer->imguiEnd();


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
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::onWindowClose, this, std::placeholders::_1));

		/*auto val = Input::getMousePos();*/
		/*HZN_CORE_TRACE("{0}, {0}", val.first, val.second);*/

		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			(*it)->onEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}
}