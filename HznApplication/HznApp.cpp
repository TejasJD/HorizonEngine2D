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
	std::string gridVertexShader = R"(
		#version 420 core
		layout (location = 0) in vec3 a_Pos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		
		void main()
		{
			gl_Position = projection * view * model * vec4(a_Pos, 1.0f);
		}
	)";

	std::string gridFragmentShader = R"(
		# version 420 core
		
		uniform vec4 a_Color;
		out vec4 fragmentColor;

		void main()
		{
			fragmentColor = a_Color;
		}
	)";

	std::string textureVertexShader = R"(
		#version 420 core
		layout (location = 0) in vec3 a_Pos;
		layout (location = 1) in vec2 a_TexCoord;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		out vec2 v_TexCoord;
		
		void main()
		{
			gl_Position = projection * view * model * vec4(a_Pos, 1.0f);
			v_TexCoord = a_TexCoord;
		}
	)";

	std::string textureFragmentShader = R"(
		# version 420 core
		
		in vec2 v_TexCoord;
		out vec4 fragmentColor;

		uniform sampler2D f_Texture;

		void main()
		{
			fragmentColor = texture(f_Texture, v_TexCoord);
		}
	)";

	Hzn::BufferLayout layout =
	{
		{Hzn::ShaderDataType::Vec3f, "a_Pos"},
		{Hzn::ShaderDataType::Vec2f, "a_TexCoord"}
	};

	std::vector<float> vertices = {
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,
		0, 3, 2
	};

	shader = std::shared_ptr<Hzn::Shader>(Hzn::Shader::create(gridVertexShader, gridFragmentShader));
	textureShader = std::shared_ptr<Hzn::Shader>(Hzn::Shader::create(textureVertexShader, textureFragmentShader));

	texture = Hzn::Texture2D::create("assets/textures/Checkerboard.png");

	textureShader->bind();
	textureShader->setUniform("f_Texture", 0);

	vertexBuffer = std::shared_ptr<Hzn::VertexBuffer>(Hzn::VertexBuffer::create(vertices));
	vertexBuffer->setBufferLayout(layout);

	elementBuffer = std::shared_ptr<Hzn::ElementBuffer>(Hzn::ElementBuffer::create(indices));

	vertexArray = std::shared_ptr<Hzn::VertexArray>(Hzn::VertexArray::create());
	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setElementBuffer(elementBuffer);

	camera = std::shared_ptr<Hzn::Camera>(new Hzn::OrthographicCamera(
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f,
		{ 0.0f, 0.0f, 3.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 1.0f, 0.0f }
	));

	camera->speed = 2.5f;
}

void SampleLayer::onAttach()
{
	HZN_CRITICAL("Sample Layer Attached!");
}

void SampleLayer::onUpdate(Hzn::TimeStep deltaTime)
{
	float currentFrame = glfwGetTime();
	++frameCount;

	if (currentFrame - previousSecond >= 1.0f)
	{
		previousSecond = currentFrame;
		fps = frameCount;
		frameCount = 0;
	}

	camera->deltaTime = deltaTime;

	if (Hzn::Input::keyPressed(Hzn::Key::Up))
	{
		camera->position += (camera->speed * camera->deltaTime) * camera->up;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Down))
	{
		camera->position -= (camera->speed * camera->deltaTime) * camera->up;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Left))
	{
		camera->position -= (camera->speed * camera->deltaTime) * glm::vec3({ 1.0f, 0.0f, 0.0f });
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Right))
	{
		camera->position += (camera->speed * camera->deltaTime) * glm::vec3({ 1.0f, 0.0f, 0.0f });;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::J))
	{
		rotation -= 90.0f * camera->deltaTime;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::L))
	{
		rotation += 90.0f * camera->deltaTime;
	}
	auto y = sin(glm::radians(rotation));
	auto x = cos(glm::radians(rotation));
	camera->up = glm::vec3(x, y, 0.0f);

	Hzn::RenderCall::setClearColor({0.1f, 0.1f, 0.1f, 0.1f});
	Hzn::RenderCall::submitClear();

	Hzn::Renderer::beginScene(camera);

	texture->bind();
	Hzn::Renderer::render(textureShader, vertexArray
	, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	shader->bind();
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((float)i * 0.11f , (float)j * 0.11f, 0));
			shader->setUniform("a_Color", glm::vec4(gridColor, 1.0f));
			model = glm::scale(model, glm::vec3(0.1f));
			Hzn::Renderer::render(shader, vertexArray, model);
		}
	}

	Hzn::Renderer::endScene();
}

void SampleLayer::onRenderImgui()
{
	static bool my_tool_active = true;
	ImGui::Begin("FPS Counter", &my_tool_active);
	ImGui::Text("Framerate: %.2f FPS", (float)fps);
	ImGui::End();

	ImGui::Begin("Color Picker");
	ImGui::ColorEdit3("Grid Color", glm::value_ptr(gridColor), true);
	ImGui::End();
}

void SampleLayer::onEvent(Hzn::Event& event)
{
	Hzn::EventDispatcher e(event);
	e.Dispatch<Hzn::MouseScrolledEvent>(std::bind(&SampleLayer::onMouseScroll, this, std::placeholders::_1));
}

void SampleLayer::onDetach()
{

}

void SampleLayer::mouseMovementCamera(Hzn::TimeStep deltaTime)
{

}

bool SampleLayer::onMouseScroll(Hzn::MouseScrolledEvent& event)
{
	/*HZN_INFO("(xOffset = {0}, yOffset = {1})", event.GetXOffset(), event.GetYOffset());*/
	return false;
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