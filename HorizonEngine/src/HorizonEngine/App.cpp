#include "pch.h"
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
			layout(location = 0) in vec3 inv_Pos;
			layout(location = 1) in vec4 inv_Col;
			out vec4 outv_Col;
			void main()
			{
				gl_Position = vec4(inv_Pos, 1.0f);
				outv_Col = inv_Col;
			}
		)";

		std::string fragmentShader = R"(
			#version 420 core
			in vec4 outv_Col;
			out vec4 outf_Col;
			void main()
			{
				outf_Col = outv_Col;
			}
		)";

		m_Shader = std::unique_ptr<Shader>(Shader::create(vertexShader, fragmentShader));
	}

	App::~App() {}

	//! the main App run loop. This loop keeps the application running and updates and renders
	//! different layers
	void App::run()
	{
		HZN_CORE_WARN("App started running...");

		std::vector<float> vertices = {
			0.0f, 0.4f, 0.0f,    1.0f, 0.0f, 1.0f, 1.0f,
			0.4f, -0.4f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,
			-0.4f, -0.4f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,
		};

		std::vector<unsigned int> indicesSquare = {
			3, 5, 6,
			3, 4, 5,
		};

		std::vector<unsigned int> indicesTriangle = {
			0, 1, 2
		};

		m_VertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create(vertices));
		m_ElementBuffer = std::shared_ptr<ElementBuffer>(ElementBuffer::create(indicesTriangle));
		m_VertexArray = std::unique_ptr<VertexArray>(VertexArray::create());

		BufferLayout layout =
		{
			{ ShaderDataType::Vec3f, "position"},
			{ ShaderDataType::Vec4f, "color"},
		};

		m_VertexBuffer->setBufferLayout(layout);
		m_VertexArray->addVertexBuffer(m_VertexBuffer);
		m_VertexArray->setElementBuffer(m_ElementBuffer);

		std::shared_ptr<VertexArray> va2 = std::shared_ptr<VertexArray>(VertexArray::create());
		va2->addVertexBuffer(m_VertexBuffer);

		std::shared_ptr<ElementBuffer> eb = std::shared_ptr<ElementBuffer>(ElementBuffer::create(indicesSquare));
		va2->setElementBuffer(eb);

		while (m_Running)
		{
			RenderCall::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCall::submitClear();

			Renderer::beginScene();

			m_Shader->bind();
			Renderer::render(va2);
			Renderer::render(m_VertexArray);

			Renderer::endScene();

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