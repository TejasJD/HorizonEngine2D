#include "pch.h"
#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "Sandbox.h"
#include "HznApp.h"



std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<HznApp>();
	app->addLayer(new EditorLayer());
	//app->addLayer(new Sandbox());

	return app;
}

// ************************************************************************

// *********** EDITOR LAYER **********
EditorLayer::EditorLayer(const char* name) : Hzn::Layer(name) {


	folderIcon = Hzn::Texture2D::create("assets/icons/DirectoryIcon.png");
	fileIcon = Hzn::Texture2D::create("assets/icons/FileIcon.png");

	//Initialize the audio system and load the files under the audio folder
	Hzn::SoundDevice::Init();


}



void EditorLayer::onAttach()
{


	HZN_INFO("Editor Layer Attached!");
	Hzn::ProjectFile* file = new Hzn::ProjectFile("assets/scenes/input.txt");
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
	drawContentBrowser();
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
			/// <summary>
			/// Create a new scene here.
			/// Create a projectFile object.
			/// create ascene object - pass in p
			/// open contenents to work with.
			/// </summary>
			/// <param name="pOpen"></param>
			if (ImGui::MenuItem("New Scene", "Ctrl+N", false)) {
				EditorLayer::NewScene();
			}
			if (ImGui::MenuItem("Open Scene", "Ctrl+O", false)) {

				std::string filePathfromdialog = Hzn::FileDialogs::openFile();
				SceneFilePath = filePathfromdialog;
				//Check if the dtring returns empty or not
				if (filePathfromdialog != "") {

					Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
					openScene = new Hzn::Scene(p);
					openScene->open();
				}
				else {
					//create new scene here
					EditorLayer::NewScene();
				}
			};

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl+S", false)) {

				//check if file path is empty
				if (SceneFilePath != "") {
					Hzn::ProjectFile* p = new Hzn::ProjectFile(SceneFilePath);
					Hzn::Scene* s = new Hzn::Scene(p);
					s->save();
				}
				else {
					//Save As here create new scene here
					EditorLayer::SaveAs();
				}
			};

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S", false)) {
				EditorLayer::SaveAs();
			};

			ImGui::Separator();

			if(ImGui::MenuItem("New Project", "Ctrl+Shift+N", false)) {
				//create
				//new scene
				
				//new directory
					//Scene dir
					//Assets Folder
						//Textures
					//project file - capable of generating the game
					
			}

			if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O", false))
			{
				std::string projectFolderPath = Hzn::FileDialogs::openFolder();


				//Check if the dtring returns empty or not
				if (projectFolderPath != "") {
					projectRootFolder = std::filesystem::path(projectFolderPath).string();
					projectPath = "Project(" + projectRootFolder + ")";
				}

				//set assets path of current project
				assetPath = projectRootFolder + "\\assets";
				m_CurrentDirectory = assetPath;

				std::map<std::string, std::string> spriteFormat;

				//create texture for image file and sprites from sprite sheets
				for (const auto& entry : std::filesystem::recursive_directory_iterator(assetPath))
				{

					if (!entry.is_directory() && entry.path().parent_path().string().find("audios") != std::string::npos)
					{

						audioFileMap.insert(std::make_pair(entry.path().string(), new Hzn::AudioSource()));
						audioFileMap.find(entry.path().string())->second->init(entry.path().string().c_str());

					}


					if (!entry.is_directory() && entry.path().string().find(".png") != std::string::npos) {
						fileIconMap.insert(std::make_pair(entry.path().string(), Hzn::Texture2D::create(entry.path().string())));

					}

					if (!entry.is_directory() && entry.path().parent_path().string().find("sprites") != std::string::npos && entry.path().string().find(".png") != std::string::npos) {

						for (const auto& metaFile : std::filesystem::recursive_directory_iterator(entry.path().parent_path())) {
							
							if (metaFile.path().string().find(".meta") != std::string::npos && metaFile.path().filename().string().substr(0, metaFile.path().filename().string().find(".")) == entry.path().filename().string().substr(0, entry.path().filename().string().find("."))) {
								std::ifstream infile(metaFile.path().c_str(), std::ifstream::binary);
								std::string line;

								while (std::getline(infile, line)) {
									std::istringstream is_line(line);
									std::string key;
									if (std::getline(is_line, key, ':'))
									{
										std::string value;

										if (std::getline(is_line, value))
										{
											spriteFormat[key] = value;
										}
									}
								}

							}
						}

						auto spriteSheet = Hzn::Texture2D::create(entry.path().string());


						for (size_t i = 0; i < std::stoi(spriteFormat.find("row")->second); i++)
						{
							for (size_t j = 0; j < std::stoi(spriteFormat.find("column")->second); j++)
							{
								std::string currentSprite = "(" + std::to_string(i) + "," + std::to_string(j) + ")";
								spriteMap.insert(std::make_pair(entry.path().string().append(currentSprite), Hzn::Sprite2D::create(spriteSheet, { i, j }, { std::stof(spriteFormat.find("width")->second),std::stof(spriteFormat.find("height")->second) })));
							}
						}




					}
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


//Methods for Editor Layer
//newScene code
void EditorLayer::NewScene() {
	Hzn::ProjectFile* p = new Hzn::ProjectFile();
	openScene = new Hzn::Scene(p);
	openScene->open();
}

//Save As code
void EditorLayer::SaveAs() {
	std::string filePathfromdialog = Hzn::FileDialogs::saveFile();

	//Check if the dtring returns empty or not
	if (filePathfromdialog != "") {
		Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
		Hzn::Scene* s = new Hzn::Scene(p);
		s->save();
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

	if (contextObject != "") {
		std::vector<std::shared_ptr<Hzn::Component>>* components = selectedObject->getComponents();

		for (int i = 0; i < components->size(); i++) {
			//HZN_CORE_DEBUG(std::any_cast<std::shared_ptr<Hzn::GameObject>>(components->at(i)->getField("gameObject"))->name);

			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

			bool open = ImGui::TreeNodeEx(components->at(i)->getComponentType().c_str(), base_flags | ImGuiTreeNodeFlags_Selected);

			if (open) {
				std::map<std::string, std::any> map = *(components->at(i)->getValues());
				for (std::map<std::string, std::any>::iterator it = map.begin(); it != map.end(); ++it) {
					//ImGui::Text(it->first.c_str());
					drawField(it->first, it->second, components->at(i));
					//char value[128] = "";
					//if (ImGui::InputText(it->first.c_str(), value, IM_ARRAYSIZE(value))) { // , ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL)) {
					//	HZN_CORE_DEBUG(value);
					//}
					// ImGui::SameLine();
				}

				ImGui::TreePop();
			}
		}

		// TODO: Add behaviours/components of currently selected object
		if (ButtonCenteredOnLine("Add Behaviour", 0.5f)) {

		}
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
		selectedObject = openScene->findGameObject(contextObject);
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		contextObject = node->item;
		selectedObject = openScene->findGameObject(contextObject);
    
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
		// (one of the two depending on the mouse m_Position)
	}

	ImGui::End();
}

void EditorLayer::drawProjectExplorerNode(const std::filesystem::path& path) {
	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

	if (path.empty()) {
		return;
	}

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{


		if (entry.path().string().find("assets") == std::string::npos) {
			continue;
		}

		ImGuiTreeNodeFlags node_flags = base_flags;

		if (entry.path().string() == projectContextObject)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		std::string name = entry.path().string();

		auto lastSlash = name.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		name = name.substr(lastSlash, name.size() - lastSlash);

		bool entryIsFile = !std::filesystem::is_directory(entry.path());
		if (entryIsFile) {
			fileIconMap.insert(std::make_pair(entry.path().string(), Hzn::Texture2D::create(entry.path().string())));
			continue;
		}


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
			projectContextObject = entry.path().string();

			if (!entryIsFile) {
				m_CurrentDirectory = projectContextObject;

			}
		}

		if (!clickStatus && !entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			clickStatus = true;
			projectContextObject = entry.path().string();
			ImGui::OpenPopup("dirContextObject");
		}

		if (!clickStatus && entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			clickStatus = true;
			projectContextObject = entry.path().string();
			ImGui::OpenPopup("projectContextObject");
		}
	}


	openContext |= ImGui::IsPopupOpen("projectContextObject");
	dirOpenContext |= ImGui::IsPopupOpen("dirContextObject");


}

void EditorLayer::drawProjectExplorer(std::string directoryPath) {

	openContext = false;
	dirOpenContext = false;
	ImGui::Begin(projectPath.c_str());

	drawProjectExplorerNode(directoryPath);

	clickStatus = false;

	if (openContext) {
		if (ImGui::IsPopupOpen("projectContextObject")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::OpenPopup("projectContextObject");

		ImGui::BeginPopup("projectContextObject");

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

			Hzn::ProjectFile pf(projectContextObject);
			pf.deleteFile(projectContextObject);
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
			pf->deleteDir(projectContextObject);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("New file", NULL, false)) {

			if (std::filesystem::exists(projectContextObject + "/new file"))
			{
				HZN_CRITICAL("new file already exists");
			}
			else {
				std::ofstream(projectContextObject + "/new file");
			}

		}

		if (ImGui::MenuItem("New folder", NULL, false)) {

			if (std::filesystem::exists(projectContextObject + "/new folder"))
			{
				HZN_CRITICAL("new folder already exists");
			}
			else {
				std::filesystem::create_directory(projectContextObject + "/new folder");
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

void EditorLayer::drawField(std::string name, std::any& value, std::shared_ptr<Hzn::Component> c) {
	char textone[512];
	memset(textone, '\0', sizeof(textone));

	try {
		std::shared_ptr<Hzn::GameObject> go = std::any_cast<std::shared_ptr<Hzn::GameObject>>(value);
		if (go) {
			strcpy(textone, go->name.c_str());

			static std::pair<std::string, std::shared_ptr<Hzn::Component>> pair(name, c);
			if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone), ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string s(textone);
				c->setField(name, openScene->findGameObject(s));
				//value = openScene->findGameObject(s);

				nodes = openScene->getHierarchy();
			}

			return;
		}
	}
	catch (const std::bad_any_cast& e) { }

	try {
		std::shared_ptr<Hzn::Component> component = std::any_cast<std::shared_ptr<Hzn::Component>>(value);
		if (component) {
			std::shared_ptr<Hzn::GameObject> go = std::any_cast<std::shared_ptr<Hzn::GameObject>>(component->getField("gameObject"));
			strcpy(textone, go->name.c_str());

			if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone), ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::shared_ptr<Hzn::GameObject> go = openScene->findGameObject(textone);
				if (go) {
					c->setField(name, go->getComponent(component->getComponentType()));

					nodes = openScene->getHierarchy();
				}
			}

			return;
		}
	}
	catch (const std::bad_any_cast& e) {}

	try {
		glm::vec2 vec = std::any_cast<glm::vec2>(value);
		std::string s = std::to_string(vec.x) + ", " + std::to_string(vec.y);
		strcpy(textone, s.c_str());

		if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone), ImGuiInputTextFlags_EnterReturnsTrue)) {
			std::string textString(textone);
			std::string xString = textString.substr(0, textString.find(","));
			std::string yString = textString.substr(textString.find(",") + 1, textString.size() - (xString.size() + 1));
			c->setField(name, glm::vec2(std::stof(xString), std::stof(yString)));
		}

		return;
	}
	catch (const std::bad_any_cast& e) {}

	try {
		float f = std::any_cast<float>(value);
		strcpy(textone, std::to_string(f).c_str());

		if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone)), ImGuiInputTextFlags_EnterReturnsTrue) {
			std::string textString = textone;
			c->setField(name, std::stof(textString));
		}

		return;
	}
	catch (const std::bad_any_cast& e) {}

	try {
		int i = std::any_cast<int>(value);
		strcpy(textone, std::to_string(i).c_str());

		if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone), ImGuiInputTextFlags_EnterReturnsTrue)) {
			std::cout << name << std::endl;

			std::string textString = textone;
			c->setField(name, (int)std::stof(textString));
		}

		return;
	}
	catch (const std::bad_any_cast& e) {}

	try {
		std::string s = std::any_cast<std::string>(value);
		strcpy(textone, s.c_str());

		if (ImGui::InputText(name.c_str(), textone, IM_ARRAYSIZE(textone)), ImGuiInputTextFlags_EnterReturnsTrue) {
			c->setField(name, std::string(textone));
		}

		return;
	}
	catch (const std::bad_any_cast& e) {}
}

int EditorLayer::gameObjectCallback(ImGuiInputTextCallbackData* data) {
	std::cout << "asd" << std::endl;

	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
	{
		std::cout << "asd" << std::endl;
		data->InsertChars(data->CursorPos, "..");
	}

	if (data->EventFlag == ImGuiInputTextFlags_EnterReturnsTrue)
	{
		std::pair<std::string, std::shared_ptr<Hzn::Component>>* pair = ((std::pair<std::string, std::shared_ptr<Hzn::Component>>*)(data->UserData));
		HZN_CORE_DEBUG(pair->first);
		//std::pair<std::string, std::shared_ptr<Hzn::Component>>* pair = ((std::pair<std::string, std::shared_ptr<Hzn::Component>>*)(data->UserData));
		/*std::string s(data->Buf);
		pair->second->setField(pair->first, openScene->findGameObject(s));*/
	}
	
	return 0;
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


void EditorLayer::drawContentBrowser() {



	projectContextObject = m_CurrentDirectory.string();
	ImGui::Begin("Content Browser");

	if (!projectRootFolder.empty()) {
		if (m_CurrentDirectory != std::filesystem::path(assetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		std::map<std::string, std::string> spriteFormat;

		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{

			if (entry.path().string().find(".meta") != std::string::npos)
			{
				continue;
			}

			const auto& path = entry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());


			std::shared_ptr<Hzn::Texture> icon;

			if (entry.is_directory()) {
				icon = folderIcon;
			}

			else if (entry.path().string().find(".png") != std::string::npos)
			{
				fileIconMap.find(entry.path().string());
				icon = fileIconMap.find(entry.path().string())->second;
			}

			else
			{
				icon = fileIcon;
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			if (entry.path().parent_path().string().find("sprites") != std::string::npos)
			{

				for (const auto& metaFile : std::filesystem::recursive_directory_iterator(entry.path().parent_path())) {

					//spriteFormat.clear();

					if (metaFile.path().string().find(".meta") != std::string::npos && metaFile.path().filename().string().substr(0, metaFile.path().filename().string().find(".")) == entry.path().filename().string().substr(0, entry.path().filename().string().find("."))) {
						std::ifstream infile(metaFile.path().c_str(), std::ifstream::binary);
						std::string line;

						while (std::getline(infile, line)) {
							std::istringstream is_line(line);
							std::string key;
							if (std::getline(is_line, key, ':'))
							{
								std::string value;

								if (std::getline(is_line, value))
								{
									spriteFormat[key] = value;
								}
							}
						}

					}
				}

				ImGui::ImageButton((ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });


				if (ImGui::Button("show sprites")) {
					ImGui::OpenPopup("showSprites");
				}

				if (ImGui::BeginPopupModal("showSprites")) {

					ImGui::Columns(columnCount, 0, false);
					for (size_t i = 0; i < std::stoi(spriteFormat.find("row")->second); i++)
					{
						for (size_t j = 0; j < std::stoi(spriteFormat.find("column")->second); j++)
						{
							std::string currentSprite = entry.path().string().append("(").append(std::to_string(i)).append(",").append(std::to_string(j)).append(")");

							std::shared_ptr<Hzn::Sprite2D> sprite = spriteMap.find(currentSprite)->second;
							ImGui::ImageButton((ImTextureID)sprite->getSpriteSheet()->getId(), { thumbnailSize, thumbnailSize }, { sprite->getTexCoords()[0].x, sprite->getTexCoords()[2].y }, { sprite->getTexCoords()[2].x, sprite->getTexCoords()[0].y });
							std::string spriteTexCoords = "(" + std::to_string(i) + "," + std::to_string(j) + ")";
							ImGui::TextWrapped(spriteTexCoords.c_str());
							ImGui::NextColumn();
						}
					}

					ImGui::Columns(1);
					if (ImGui::Button("Close", ImVec2(120, 40))) {
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

			}
			else {
				ImGui::ImageButton((ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}

			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource()) {

				auto relativePath = std::filesystem::relative(path, assetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}


			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (entry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
	}


	ImGui::Columns(1);

	ImGui::End();
}