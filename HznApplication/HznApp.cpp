#include "pch.h"

#include <HorizonEngine.h>
#include <HznEntryPoint.h>
#include "HznApp.h"
#include <commdlg.h>
#include "FileManagement/ProjectFile.h"
#include "SceneManagement/Scene.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<HznApp>();
	app->addLayer(new SampleLayer());
	//app->addLayer(new EditorLayer());
	return app;
}

// *********** SAMPLE LAYER **********

SampleLayer::SampleLayer(const std::string& name) : Layer(name)
{
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

	m_Shader = std::shared_ptr<Hzn::Shader>(Hzn::Shader::create(vertexShader, fragmentShader));

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
		5, 1, 2,
		5, 6, 2,
		4, 5, 1,
		4, 0, 1,
		7, 3, 2,
		7, 6, 2
	};

	m_VertexBuffer = std::shared_ptr<Hzn::VertexBuffer>(Hzn::VertexBuffer::create(cube));
	m_ElementBuffer = std::shared_ptr<Hzn::ElementBuffer>(Hzn::ElementBuffer::create(cubeIndex));
	m_VertexArray = std::unique_ptr<Hzn::VertexArray>(Hzn::VertexArray::create());

	Hzn::BufferLayout layout =
	{
		{ Hzn::ShaderDataType::Vec3f, "position"},
		{ Hzn::ShaderDataType::Vec4f, "color"},
	};

	m_VertexBuffer->setBufferLayout(layout);
	m_VertexArray->addVertexBuffer(m_VertexBuffer);
	m_VertexArray->setElementBuffer(m_ElementBuffer);

	cubePositions = {
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
	camera = std::shared_ptr<Hzn::Camera>(new Hzn::Camera(cameraPosition, cameraFront, cameraUp));
	camera->setFov(45.0f);
	float windowWidth = Hzn::App::getApp().getAppWindow().getWidth();
	float windowHeight = Hzn::App::getApp().getAppWindow().getHeight();
	camera->setAspectRatio(windowWidth / windowHeight);
	camera->setSpeed(2.5f);
}

void SampleLayer::onAttach()
{
	HZN_CRITICAL("Sample Layer Attached!");
}

void SampleLayer::onUpdate(Hzn::TimeStep deltaTime)
{
	Hzn::RenderCall::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Hzn::RenderCall::submitClear();

	Hzn::Renderer::beginScene(camera);

	m_Shader->bind();
	float currentFrame = glfwGetTime();
	++frameCount;

	if (currentFrame - previousSecond >= 1.0f)
	{
		previousSecond = currentFrame;
		fps = frameCount;
		frameCount = 0;
	}

	camera->setDeltaTime(deltaTime);

	if (Hzn::Input::keyPressed(Hzn::Key::Up))
	{
		camera->moveFront();
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Down))
	{
		camera->moveBack();
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Left))
	{
		camera->moveLeft();
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Right))
	{
		camera->moveRight();
	}
	/*glm::mat4 view = cameragetViewMatrix();
	m_Hzn::Shader->setUniform("view", view);
	m_Hzn::Shader->setUniform("projection", projection);*/

	// model matrix location in vertex Hzn::Shader is obtained.
	for (size_t i = 0; i < cubePositions.size(); ++i)
	{
		glm::mat4 model = glm::mat4(1.0f); // model matrix for cube
		model = glm::translate(model, cubePositions[i]);
		float angle = 10.0f * (i + 1);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader->setUniform("model", model);
		// draw the vertex array.
		Hzn::Renderer::render(m_Shader, m_VertexArray);
	}

	Hzn::Renderer::endScene();
}

void SampleLayer::onRenderImgui()
{
	static bool my_tool_active = true;
	ImGui::Begin("FPS Counter", &my_tool_active);
	ImGui::Text("Framerate: %.2f FPS", (float)fps);
	ImGui::End();
}

void SampleLayer::onDetach()
{

}

void SampleLayer::onEvent(Hzn::Event& event)
{
	Hzn::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hzn::KeyPressedEvent>(std::bind(&SampleLayer::onKeyPressedEvent, this, std::placeholders::_1));
}

// ************************************************************************

// *********** EDITOR LAYER **********
EditorLayer::EditorLayer(const std::string& name) : Layer(name) {}

void EditorLayer::onAttach()
{
	HZN_INFO("Editor Layer Attached!");
}

void EditorLayer::onRenderImgui()
{
	ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
	ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y));

	static bool my_tool_active = true;

	if (ImGui::Begin(
		"From Editor Layer",
		nullptr,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoTitleBar
	)
		)

	{
		{
			static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
			ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
			ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene", "Ctrl+N", false)) {

					}



					if (ImGui::MenuItem("Open Scene", "Ctrl+O", false)) {

						std::string filePathfromdialog = Hzn::FileDialogs::openFile();

						//Check if the dtring returns empty or not
						if (filePathfromdialog != "") {


							Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
							Hzn::Scene* s = new Hzn::Scene(p);
							s->open();
						}
						else {
							//create new scene here
						}
					};


					ImGui::Separator();



					if (ImGui::MenuItem("Save", "Ctrl+S", false)) {

						std::string filePathfromdialog = Hzn::FileDialogs::saveFile();

						//Check if the dtring returns empty or not
						if (filePathfromdialog != "") {
							Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
							Hzn::Scene* s = new Hzn::Scene(p);
							s->save();
						}
						else {
							//create new scene here
						}


					};

					if (ImGui::MenuItem("Save As", "Ctrl+Shift+S", false)) {


					};

					ImGui::Separator();



					ImGui::MenuItem("New Project", "Ctrl+Shift+N", false);
					ImGui::MenuItem("Open Project", "Ctrl+Shift+O", false);
					ImGui::Separator();



					ImGui::MenuItem("Build Settings", NULL, false);
					ImGui::MenuItem("Build and Run", NULL, false);
					ImGui::Separator();



					//if (ImGui::MenuItem("Exit", NULL, false, pOpen != NULL))
					//*pOpen = false;
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do Something */ }
					if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do Something */ }

					if (ImGui::MenuItem("Cut", "Ctrl+X")) { /*Do Something*/ }
					if (ImGui::MenuItem("Copy", "Ctrl+C")) { /*Do Something*/ }
					if (ImGui::MenuItem("Paste", "Ctrl+V")) { /*Do Something*/ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Assets"))
				{
					if (ImGui::MenuItem("Asset Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Game Objects"))
				{
					if (ImGui::MenuItem("Game Object Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Components"))
				{
					if (ImGui::MenuItem("Components Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Window"))
				{
					if (ImGui::MenuItem("Window Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("Help Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
			}
			ImGui::EndMainMenuBar();
		}
	}
	ImGui::End();
}


void EditorLayer::onDetach() {}

void EditorLayer::onEvent(Hzn::Event& event)
{
	if (event.GetTypeOfEvent() == Hzn::TypeOfEvent::KeyPressed)
	{
		Hzn::KeyPressedEvent& e = (Hzn::KeyPressedEvent&)event;
		auto key = (char)e.GetKeyCode();
		HZN_INFO("{0}", key);
	}
}