#include "pch.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "HorizonEngine/Camera.h"

#include "App.h"

namespace Hzn
{
	App* App::m_Instance = nullptr;
	//! App class constructor, initializes the application
	App::App() : m_Running(true)
	{
		registerComponents();
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

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			void main()
			{
				gl_Position = projection * view * model * vec4(inv_Pos, 1.0f);
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

		std::vector<float> cube = {
			0.5f, 0.5f, 0.5f,	    1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f,	    0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f,	    0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f,	    1.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f,	    1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,	    0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f,	    1.0f, 1.0f, 0.0f, 1.0f
		};

		std::vector<float> vertices = {
			0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f,
		};

		std::vector<uint32_t> indices = {
			0, 1, 2
		};

		std::vector<uint32_t> cubeIndex = {
			0, 1, 2, // forward two triangles (front face)
			0, 2, 3,
			4, 5, 6, // back two triangles (back face)
			4, 6, 7,
			4, 0, 3, // top face
			4, 7, 3,
			5, 1, 2, // back face
			5, 6, 2,
			4, 5, 1,
			4, 0, 1,
			7, 3, 2,
			7, 6, 2
		};

		m_VertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create(cube));
		m_ElementBuffer = std::shared_ptr<ElementBuffer>(ElementBuffer::create(cubeIndex));
		m_VertexArray = std::unique_ptr<VertexArray>(VertexArray::create());

		BufferLayout layout =
		{
			{ ShaderDataType::Vec3f, "position"},
			{ ShaderDataType::Vec4f, "color"},
		};

		m_VertexBuffer->setBufferLayout(layout);
		m_VertexArray->addVertexBuffer(m_VertexBuffer);
		m_VertexArray->setElementBuffer(m_ElementBuffer);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 3.0f };
		glm::vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
		glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
		std::shared_ptr<Camera> camera(new Camera(cameraPosition, cameraFront, cameraUp));
		camera->setFov(45.0f);
		camera->setAspectRatio((float)m_Window->getWidth() / (float)m_Window->getHeight());
		// projection matrix
		/*glm::mat4 projection = camera->getPerspectiveProjectionMatrix(
			45.0f,
			(float)m_Window->getWidth() / m_Window->getHeight(),
			0.1f,
			100.0f
		);*/

		/*glm::mat4 projection = camera->getOrthographicProjectionMatrix(
			-2.0f,
			2.0f,
			-2.0f,
			2.0f
		);*/

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		float previousSecond = 0.0f; // Keeps track of previous second
		float frameCount = 0.0f;
		float fps = 0;

		while (m_Running)
		{
			RenderCall::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCall::submitClear();

			Renderer::beginScene(camera);

			m_Shader->bind();

			float currentFrame = glfwGetTime();
			++frameCount;
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (currentFrame - previousSecond >= 1.0f)
			{
				previousSecond = currentFrame;
				fps = frameCount;
				frameCount = 0;
			}

			camera->setSpeed(2.5f);
			camera->setDeltaTime(deltaTime);
			/*HZN_CORE_INFO(cameraSpeed);*/
			if (Input::keyPressed(GLFW_KEY_W))
			{
				camera->moveFront();
			}
			else if (Input::keyPressed(GLFW_KEY_S))
			{
				camera->moveBack();
			}
			else if (Input::keyPressed(GLFW_KEY_A))
			{
				camera->moveLeft();
			}
			else if (Input::keyPressed(GLFW_KEY_D))
			{
				camera->moveRight();
			}

			/*glm::mat4 view = camera->getViewMatrix();
			m_Shader->setUniform("view", view);
			m_Shader->setUniform("projection", projection);*/

			// model matrix location in vertex shader is obtained.
			for (size_t i = 0; i < 10; ++i)
			{
				glm::mat4 model = glm::mat4(1.0f); // model matrix for cube
				model = glm::translate(model, cubePositions[i]);
				float angle = 10.0f * (i + 1);
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				m_Shader->setUniform("model", model);
				// draw the vertex array.
				Renderer::render(m_Shader, m_VertexArray);
			}

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

			static bool my_tool_active = true;
			ImGui::Begin("FPS Counter", &my_tool_active);
			ImGui::Text("Framerate: %.2f FPS", (float)fps);
			ImGui::End();

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

	void App::registerComponents() {
		REGISTER(Component, Transform);
		REGISTER(Component, BoxCollider2D);
		REGISTER(Component, Rigidbody2D);
	}
}