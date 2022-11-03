#include "pch.h"

#include <HorizonEngine.h>
#include <HznEntryPoint.h>
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<HznApp>();
	app->addLayer(new EditorLayer());
	//app->addLayer(new SampleLayer());
	return app;
}

// *********** SAMPLE LAYER **********

SampleLayer::SampleLayer(const std::string& name) : Layer(name)
{
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

	shader = std::shared_ptr<Hzn::Shader>(Hzn::Shader::create({
		{Hzn::ShaderType::VertexShader, "assets/shaders/GridVertex.glsl"},
		{Hzn::ShaderType::FragmentShader, "assets/shaders/GridFragment.glsl"}
	}));
	textureShader = std::shared_ptr<Hzn::Shader>(Hzn::Shader::create({
		{Hzn::ShaderType::VertexShader, "assets/shaders/TextureVertex.glsl"},
		{Hzn::ShaderType::FragmentShader, "assets/shaders/TextureFragment.glsl"}
	}));

	texture = Hzn::Texture2D::create("assets/textures/Checkerboard.png");
	logoTexture = Hzn::Texture2D::create("assets/textures/someSky.png");

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

	texture->bind();
	Hzn::Renderer::render(textureShader, vertexArray
		, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	logoTexture->bind();
	Hzn::Renderer::render(textureShader, vertexArray
		, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
EditorLayer::EditorLayer(const std::string& name) : Layer(name) {
	
	//Initialize the audio system and load the files under the audio folder
	Hzn::SoundDevice::Init();

	for (const auto& entry : std::filesystem::directory_iterator("assets/audios/"))
	{

		audioFileMap.insert(std::make_pair(entry.path().string(), new Hzn::AudioSource()));
		audioFileMap.find(entry.path().string())->second->init(entry.path().string().c_str());

	}
}



void EditorLayer::onAttach()
{
	

	HZN_INFO("Editor Layer Attached!");
	Hzn::ProjectFile *file = new Hzn::ProjectFile("assets/scenes/input.txt");
	openScene = new Hzn::Scene(file);
	openScene->open();
	nodes = openScene->getHierarchy();
	/*delete file;*/
}

void EditorLayer::onRenderImgui()
{
	ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
	ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y));

	static bool my_tool_active = true;

	//if (ImGui::Begin(
	//	"From Editor Layer",
	//	nullptr,
	//	ImGuiWindowFlags_MenuBar |
	//	ImGuiWindowFlags_NoResize |
	//	ImGuiWindowFlags_NoMove |
	//	ImGuiWindowFlags_NoCollapse |
	//	ImGuiWindowFlags_NoBringToFrontOnFocus |
	//	ImGuiWindowFlags_NoTitleBar
	//)
	//	)

	//{
	//	{
	//		static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	//		ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
	//		ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);

	//		if (ImGui::BeginMainMenuBar())
	//		{
	//			if (ImGui::BeginMenu("File"))
	//			{
	//				if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do Something */ }
	//				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do Something */ }
	//				if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Edit"))
	//			{
	//				if(ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do Something */ }
	//				if(ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do Something */ }

	//				if(ImGui::MenuItem("Cut", "Ctrl+X")){ /*Do Something*/ }
	//				if (ImGui::MenuItem("Copy", "Ctrl+C")) { /*Do Something*/ }
	//				if (ImGui::MenuItem("Paste", "Ctrl+V")) { /*Do Something*/ }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Assets"))
	//			{
	//				if(ImGui::MenuItem("Asset Submenu_1")){ /* Do Something */ }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Game Objects"))
	//			{
	//				if (ImGui::MenuItem("Game Object Submenu_1")) { /* Do Something */ }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Components"))
	//			{
	//				if (ImGui::MenuItem("Components Submenu_1")) { /* Do Something */ }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Window"))
	//			{
	//				if (ImGui::MenuItem("Window Submenu_1")) { /* Do Something */ }
	//				ImGui::EndMenu();
	//			}
	//			if (ImGui::BeginMenu("Help"))
	//			{
	//				if (ImGui::MenuItem("Help Submenu_1")) { /* Do Something */ }
	//				ImGui::EndMenu();
	//			}
	//		}
	//		ImGui::EndMainMenuBar();
	//	}
	//}
	//ImGui::End();

	// Setup dock space
	bool pOpen = true;
	setupDockSpace(&pOpen);
	drawScene();
	drawObjectBehaviour();
	drawHierarchy();
	drawProjectExplorer(projectRootFolder);
	drawConsole();
	drawAudio("assets/audios/");
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

void EditorLayer::setupDockSpace(bool* pOpen) {
	bool fullscreen = true;
	bool padding = false;
	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiConfigFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	if (!padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", pOpen, windowFlags);
	if (!padding)
		ImGui::PopStyleVar();

	if (fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

		dockWidgets(dockspace_id);

		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	drawMenuBar(pOpen);

	ImGui::End();
}

void EditorLayer::dockWidgets(ImGuiID dockspace_id) {
	if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
		// Clear out existing layout
		ImGui::DockBuilderRemoveNode(dockspace_id);
		// Add empty node
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		// Main node should cover entire window
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());
		// Build dock layout
		ImGuiID center = dockspace_id;
		ImGuiID left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.15f, nullptr, &center);
		ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.15f, nullptr, &center);
		ImGuiID down = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.15f, nullptr, &center);

		ImGuiID rightDown;
		ImGuiID rightUp = ImGui::DockBuilderSplitNode(right, ImGuiDir_Up, 0.5f, nullptr, &rightDown);

		ImGui::DockBuilderDockWindow("Scene", center);
		ImGui::DockBuilderDockWindow("Object Behaviour", left);
		//ImGui::DockBuilderDockWindow(projectPath.c_str(), rightUp);
		//ImGui::DockBuilderDockWindow("Audio", rightUp);
		ImGui::DockBuilderDockWindow("Hierarchy", rightDown);
		ImGui::DockBuilderDockWindow("Console", down);

		ImGui::DockBuilderFinish(dockspace_id);
	}
}

void EditorLayer::drawMenuBar(bool* pOpen) {
	if (ImGui::BeginMenuBar())
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

			if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O", false))
			{
				std::string projectFolderPath = Hzn::FileDialogs::openFolder();


				//Check if the dtring returns empty or not
				if (projectFolderPath != "") {
					projectRootFolder = std::filesystem::path(projectFolderPath).string();
					projectPath = "Project(" + projectRootFolder + ")";
				}
			}

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
			ImGui::Separator();

			ImGui::MenuItem("Select All", "Ctrl+A", false);
			ImGui::MenuItem("Deselect All", NULL, false);
			ImGui::Separator();

			ImGui::MenuItem("Cut", "Ctrl+X", false);
			ImGui::MenuItem("Copy", "Ctrl+C", false);
			ImGui::MenuItem("Paste", "Ctrl+V", false);
			ImGui::MenuItem("Duplicate", "Ctrl+D", false);
			ImGui::MenuItem("Rename", "Ctrl+R", false);
			ImGui::MenuItem("Delete", "Delete", false);
			ImGui::Separator();

			ImGui::MenuItem("Play", NULL, false);
			ImGui::MenuItem("Pause", NULL, false);
			ImGui::Separator();

			ImGui::MenuItem("Project Settings", NULL, false);
			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Scene", NULL, true);
			ImGui::MenuItem("Hierarchy", NULL, true);
			ImGui::MenuItem("Object Properties", NULL, true);
			ImGui::MenuItem("Project", NULL, true);
			ImGui::MenuItem("Console", NULL, true);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void EditorLayer::drawScene() {
	if (ImGui::Begin("Scene"))
	{
		static ImVec2 scrolling(0.0f, 0.0f);
		static bool opt_enable_grid = true;

		ImGui::Checkbox("Enable grid", &opt_enable_grid);

		// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
		// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
		// To use a child window instead we could use, e.g:
		//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
		//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
		//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
		//      ImGui::PopStyleColor();
		//      ImGui::PopStyleVar();
		//      [...]
		//      ImGui::EndChild();

		// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
		draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

		// This will catch our interactions
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held
		const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
		const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

		// Pan (we use a zero mouse threshold when there's no context menu)
		// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
		const float mouse_threshold_for_pan = -1.0f;
		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}

		// Context menu (under default mouse threshold)
		ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
			ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopup("context"))
		{
			if (ImGui::MenuItem("Context Item 1", NULL, false)) {
				// Do stuff here
			}
			if (ImGui::MenuItem("Context Item 2", NULL, false)) {
				// Do stuff here 
			}
			ImGui::EndPopup();
		}

		// Draw grid
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);
		if (opt_enable_grid)
		{
			const float GRID_STEP = 64.0f;
			for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
			for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
		}
		draw_list->PopClipRect();

		ImGui::End();
	}
}

void EditorLayer::drawObjectBehaviour() {
	//bool pOpen = true;
	//ImGuiConfigFlags configFlags = ImGuiDir_Right | ImGuiWindowFlags_NoCollapse;
	//ImGui::Begin("Object Behaviour", &pOpen, configFlags);
	ImGui::Begin("Object Behaviour");
	// TODO: Add behaviours/components of currently selected object
	if (ButtonCenteredOnLine("Add Behaviour", 0.5f)) {

	}
	ImGui::End();
}

void EditorLayer::drawHierarchyNode(std::shared_ptr<Hzn::TreeNode<std::string>> node) {
	//ImGui::Text(text.c_str());

	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

	if (node->nextNodes.size() == 0)
		base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	if (node->item == contextObject) {
		base_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool open = ImGui::TreeNodeEx(node->item.c_str(), base_flags);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		contextObject = node->item;
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		contextObject = node->item;
		ImGui::OpenPopup("contextObject");
	}

	std::string s = ImGui::IsPopupOpen("contextObject") ? "true" : "false";
	
	openContext |= ImGui::IsPopupOpen("contextObject");

	if (open) {
		for (int i = 0; i < node->nextNodes.size(); i++) {
			drawHierarchyNode(node->nextNodes.at(i));
		}
	}

	if (open && node->nextNodes.size() != 0)
		ImGui::TreePop();
}

void EditorLayer::drawHierarchy() {
	ImGui::Begin("Hierarchy");

	openContext = false;

	for (int i = 0; i < nodes.size(); i++) {
		drawHierarchyNode(nodes.at(i));
	}

	if (openContext) {
		if (ImGui::IsPopupOpen("contextObject")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::OpenPopup("contextObject");

		ImGui::BeginPopup("contextObject");
		//HZN_CORE_DEBUG("Object: " + contextObject);

		if (ImGui::MenuItem("Copy", NULL, false)) {
			/*copiedGameObject = openScene->findGameObject(contextObject);*/
		}
		if (ImGui::MenuItem("Paste", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Duplicate", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Delete", NULL, false)) {
			/*openScene->removeGameObject(contextObject);
			nodes = openScene->getHierarchy();*/
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Create Empty", NULL, false)) {
			openScene->createEmpty(contextObject);
			nodes = openScene->getHierarchy();
		}

		ImGui::EndPopup();
	}

	// Right-click
	ImVec2 emptySpaceSize = ImGui::GetContentRegionAvail();
	if (emptySpaceSize.x < 50) emptySpaceSize.x = 50;
	if (emptySpaceSize.y < 50) emptySpaceSize.y = 50;
	ImGui::InvisibleButton("canvas", emptySpaceSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	// Context menu (under default mouse threshold)
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
		ImGui::OpenPopupOnItemClick("contextHierarchy", ImGuiPopupFlags_MouseButtonRight);
	}
	if (ImGui::BeginPopup("contextHierarchy")) {
		contextObject = "";

		if (ImGui::MenuItem("Create Empty", NULL, false)) {
			openScene->createEmpty();
			nodes = openScene->getHierarchy();
		}

		ImGui::EndPopup();
	}

	// Left-click drag
	drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
	if (drag_delta.x > 0.0f || drag_delta.y > 0.0f) {
		// TODO: Put dragged object as child of currently hovered object
		// or change its posiiton in the hierarchy 
		// (one of the two depending on the mouse position)
	}

	ImGui::End();
}

void EditorLayer::drawProjectExplorerNode(const std::filesystem::path& path){
	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

	if (path.empty()) {
		return ;
	}

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{

		
		if (entry.path().string().find("assets") == std::string::npos) {
			continue;
		}

		ImGuiTreeNodeFlags node_flags = base_flags;
		
		if (entry.path().string() == contextObject)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		std::string name = entry.path().string();

		auto lastSlash = name.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		name = name.substr(lastSlash, name.size() - lastSlash);

		bool entryIsFile = !std::filesystem::is_directory(entry.path());
		if (entryIsFile)
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

		if (!entryIsFile)
		{
			if (node_open)
			{
				drawProjectExplorerNode(entry.path());

				ImGui::TreePop();
			}
		}

		if (!clickStatus && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			clickStatus = true;
			contextObject = entry.path().string();
		}

		if (!clickStatus && !entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			clickStatus = true;
			contextObject = entry.path().string();
			ImGui::OpenPopup("dirContextObject");
		}

		if (!clickStatus && entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			clickStatus = true;
			contextObject = entry.path().string();
			ImGui::OpenPopup("contextObject");
		}
	}


	openContext |= ImGui::IsPopupOpen("contextObject");
	dirOpenContext |= ImGui::IsPopupOpen("dirContextObject");


}

void EditorLayer::drawProjectExplorer(std::string directoryPath){

	openContext = false;
	dirOpenContext = false;
	ImGui::Begin(projectPath.c_str());

	drawProjectExplorerNode(directoryPath);

	clickStatus = false;

	if (openContext) {
		if (ImGui::IsPopupOpen("contextObject")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::OpenPopup("contextObject");

		ImGui::BeginPopup("contextObject");

		if (ImGui::MenuItem("Cut", NULL, false)) {
			// Do stuff here
		}
		if (ImGui::MenuItem("Copy", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Paste", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Duplicate", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Rename", NULL, false)) {
			// Do stuff here 
			
		}
		if (ImGui::MenuItem("Delete")) {
			
			Hzn::ProjectFile pf(contextObject);
			pf.deleteFile(contextObject);
		}
	
		ImGui::EndPopup();
	}

	if (dirOpenContext) {
		if (ImGui::IsPopupOpen("dirContextObject")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::OpenPopup("dirContextObject");

		ImGui::BeginPopup("dirContextObject");

		if (ImGui::MenuItem("Cut", NULL, false)) {
			// Do stuff here
		}
		if (ImGui::MenuItem("Copy", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Paste", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Duplicate", NULL, false)) {
			// Do stuff here 
		}
		if (ImGui::MenuItem("Rename", NULL, false)) {
			// Do stuff here 

		}
		if (ImGui::MenuItem("Delete", NULL, false)) {
			
			Hzn::ProjectFile* pf = nullptr;
			pf->deleteDir(contextObject);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("New file", NULL, false)) {

			if (std::filesystem::exists(contextObject + "/new file"))
			{
				HZN_CRITICAL("new file already exists");
			}
			else {
				std::ofstream(contextObject + "/new file");
			}

		}

		if (ImGui::MenuItem("New folder", NULL, false)) {

			if (std::filesystem::exists(contextObject + "/new folder"))
			{
				HZN_CRITICAL("new folder already exists");
			}
			else {
				std::filesystem::create_directory(contextObject + "/new folder");
			}

		}

		ImGui::EndPopup();
	}

	// Right-click
	ImVec2 emptySpaceSize = ImGui::GetContentRegionAvail();
	if (emptySpaceSize.x < 50) emptySpaceSize.x = 50;
	if (emptySpaceSize.y < 50) emptySpaceSize.y = 50;
	ImGui::InvisibleButton("canvas", emptySpaceSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	// Context menu (under default mouse threshold)
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
		ImGui::OpenPopupOnItemClick("contextProject", ImGuiPopupFlags_MouseButtonRight);
	}
	if (ImGui::BeginPopup("contextProject")) {
		

		if (ImGui::MenuItem("New file", NULL, false)) {
			if (std::filesystem::exists(projectRootFolder + "/new file"))
			{
				HZN_CRITICAL("new file already exists");
			}
			else {
				std::ofstream(projectRootFolder + "/new file");
			}
		}

		if (ImGui::MenuItem("New folder", NULL, false)) {

			if (std::filesystem::exists(projectRootFolder + "/new folder"))
			{
				HZN_CRITICAL("new folder already exists");
			}
			else {
				std::filesystem::create_directory(projectRootFolder + "/new folder");
			}

		}


		ImGui::EndPopup();
	}

	ImGui::End();
}

void EditorLayer::drawConsole() {
	ImGui::Begin("Console");
	ImGui::Text("Some text here");
	ImGui::End();
}

// A method to horizontally align a button in the current context
bool EditorLayer::ButtonCenteredOnLine(const char* label, float alignment)
{
	float buttonWidth = 200.f;
	float buttonHeight = 30.f;

	ImGuiStyle& style = ImGui::GetStyle();

	// Use this if you don't want a fixed button size
	//float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float size = buttonWidth;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label, ImVec2(buttonWidth, buttonHeight));
}



void EditorLayer::drawAudioNode(const std::filesystem::path& path) {
	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

	

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		
		ImGuiTreeNodeFlags node_flags = base_flags;

		if (entry.path().string() == contextObject)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		std::string name = entry.path().string();

		auto lastSlash = name.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		name = name.substr(lastSlash, name.size() - lastSlash);

		bool entryIsFile = !std::filesystem::is_directory(entry.path());
		if (entryIsFile)
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			contextObject = entry.path().string();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			contextObject = entry.path().string();

			ImGui::OpenPopup("contextObject");
		}


	}


	openContext |= ImGui::IsPopupOpen("contextObject");

}

void EditorLayer::drawAudio(std::string directoryPath) {
	

	openContext = false;
	ImGui::Begin("Audios");
	
	drawAudioNode(directoryPath);

	if (openContext) {
		if (ImGui::IsPopupOpen("contextObject")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::OpenPopup("contextObject");

		ImGui::BeginPopup("contextObject");

		if (ImGui::MenuItem("Play", NULL, false)) {
			audioFileMap.find(contextObject)->second->Play();
		}
		if (ImGui::MenuItem("Pause", NULL, false)) {
			audioFileMap.find(contextObject)->second->Pause();
		}
		if (ImGui::MenuItem("Resume", NULL, false)) {
			audioFileMap.find(contextObject)->second->Resume();
		}
		if (ImGui::MenuItem("Stop", NULL, false)) {
			audioFileMap.find(contextObject)->second->Stop();
		}

		ImGui::EndPopup();
	}

	// Right-click
	ImVec2 emptySpaceSize = ImGui::GetContentRegionAvail();
	if (emptySpaceSize.x < 50) emptySpaceSize.x = 50;
	if (emptySpaceSize.y < 50) emptySpaceSize.y = 50;
	ImGui::InvisibleButton("canvas", emptySpaceSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	ImGui::End();
}