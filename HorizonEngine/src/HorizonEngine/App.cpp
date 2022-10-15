#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/GLShader.h"

#include "App.h"

namespace Hzn
{

	App* App::m_Instance = nullptr;
	//! App class constructor, initializes the application
	App::App() : m_Running(true)
	{
		/*HZN_CORE_ASSERT(false, "application already initialized");*/
		m_Instance = this;
		m_Input = std::unique_ptr<Input>(Input::create());
		m_Window = std::unique_ptr<Window>(Window::create(800, 600, "HorizonEngine"));
		m_Window->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));

		m_ImguiLayer = new ImguiLayer();
		addOverlay(m_ImguiLayer);

		std::string vertexShader = R"(
			#version 420 core
			layout(location = 0) in vec3 pos;
			out vec3 posCol;
			void main()
			{
				gl_Position = vec4(pos, 1.0f);
				posCol = pos;
			}
		)";
		
		std::string fragmentShader = R"(
			#version 420 core
			in vec3 posCol;
			out vec4 color;
			void main()
			{
				color = vec4((posCol + 1.0) * 0.5, 1.0f);
			}
		)";

		m_Shader = std::unique_ptr<Shader>(Shader::create(vertexShader, fragmentShader));
	}

	App::~App()
	{
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_ElementBuffer);
	}

	//! the main App run loop. This loop keeps the application running and updates and renders
	//! different layers
	void App::run()
	{
		HZN_CORE_WARN("App started running...");

		float vertices[9] = {
			0.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
		};

		unsigned int indices[3] = {
			0, 1, 2
		};

		glGenVertexArrays(1, &m_VertexArray);
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_ElementBuffer);

		glBindVertexArray(m_VertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		while (m_Running)
		{
			m_Shader->use();
			glBindVertexArray(m_VertexArray);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			//! general layer update
			for (auto& layer : m_Layers)
			{
				layer->onUpdate();
			}
			
			//! updates UI components on any layers
			m_ImguiLayer->imguiBegin();
			for (auto& layer : m_Layers)
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