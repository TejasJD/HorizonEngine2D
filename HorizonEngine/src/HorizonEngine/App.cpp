#include "pch.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "App.h"

#include "HorizonEngine/Renderer/Renderer.h"
#include "HorizonEngine/Scripting/ScriptEngine.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/SceneManagement/FunctionRegistry.h"

namespace Hzn
{
	App* App::m_Instance = nullptr;
	//! App class constructor, initializes the application
	App::App() : m_Running(true)
	{
		m_ExecutablePath = std::filesystem::current_path();
		HZN_CORE_CRITICAL("ExecutablePath: {}", m_ExecutablePath.string());
		m_Instance = this;

		m_Window = Window::create(1920, 1080, "HorizonEngine");
		// set the App on event function as callback for the widow class.
		m_Window->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));

		m_Window->setVsync(true);

		Renderer::init();

		m_ImguiLayer = new ImguiLayer();
		addOverlay(m_ImguiLayer);

		RegisterComponentFunctions(AllComponents{});
	}

	App::~App() {}

	//! the main App run loop. This loop keeps the application running and updates and renders
	//! different layers
	void App::run()
	{
		HZN_CORE_WARN("App started running...");

		while (m_Running)
		{
			const auto currentFrameTime = static_cast<const float>(glfwGetTime());
			const TimeStep deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			executeMainThreadQueue();

			//! general layer update
			if (!m_Minimized) 
			{
				for (const auto& layer : m_Layers)
				{
					layer->onUpdate(deltaTime);
				}
			}

			//! updates UI components on any layers
			m_ImguiLayer->imguiBegin();
			for (const auto& layer : m_Layers)
			{
				layer->onRenderImgui();
			}
			m_ImguiLayer->imguiEnd();

			m_Window->onUpdate();
		}
	}

	//! terminates the App run loop
	//! always returns true
	bool App::onWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool App::onWindowResize(WindowResizeEvent& e)
	{
		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}
		Renderer::onWindowResize(e.GetWidth(), e.GetHeight());
		m_Minimized = false;
		return false;
	}

	void App::executeMainThreadQueue()
	{
		for(auto& fn : m_MainThreadQueue)
		{
			fn();
		}
		m_MainThreadQueue.clear();
	}

	void App::submitMainThreadQueue(const std::function<void()>& fn)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueLock);
		m_MainThreadQueue.emplace_back(fn);
	}

	//! the onEvent function of application class that handles any events coming to the application
	void App::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::onWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&App::onWindowResize, this, std::placeholders::_1));

		/*auto val = Input::getMousePos();*/
		/*HZN_CORE_TRACE("{0}, {0}", val.first, val.second);*/

		for (const auto& layer : m_Layers)
		{
			if (e.Handled)
			{
				break;
			}
			layer->onEvent(e);
		}
	}
}
