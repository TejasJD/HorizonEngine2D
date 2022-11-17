#include <pch.h>
#include "EditorLayer.h"


EditorLayer::EditorLayer(const char* name) :
	Hzn::Layer(name),
    m_AspectRatio(static_cast<float>(Hzn::App::getApp().getAppWindow().getWidth()) /
        static_cast<float>(Hzn::App::getApp().getAppWindow().getHeight())),
	m_EditorCameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
{
}


EditorLayer::~EditorLayer()
{
	// I think it's fine for detach function to be statically resolved at compile time in this case.
	onDetach();
}

void EditorLayer::onAttach()
{
	HZN_TRACE("Editor Layer Attached!");
	m_CheckerboardTexture = Hzn::Texture2D::create("assets/textures/bear.png");
	Hzn::FrameBufferProps props;
	props.width = Hzn::App::getApp().getAppWindow().getWidth();
	props.height = Hzn::App::getApp().getAppWindow().getHeight();
	props.attachments = {
		Hzn::FrameBufferTextureFormat::RGBA8,
		Hzn::FrameBufferTextureFormat::DEPTH24_STENCIL8
	};

	m_FrameBuffer = Hzn::FrameBuffer::create(props);

	m_Scene = Hzn::SceneManager::load();

	Hzn::GameObject object0 = m_Scene->createGameObject("square 1");
	object0.addComponent<Hzn::TransformComponent>();
	object0.addComponent<Hzn::RenderComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	Hzn::GameObject object1 = m_Scene->createGameObject("square 2");

	object1.addComponent<Hzn::TransformComponent>(glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(1.0f));
	object1.addComponent<Hzn::RenderComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	Hzn::GameObject object3 = m_Scene->createGameObject("square 3");

	object3.addComponent<Hzn::TransformComponent>(glm::vec3(-3.0f, -3.0f, 0.0f), glm::vec3(1.0f));
	object3.addComponent<Hzn::RenderComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	Hzn::GameObject camera = m_Scene->createGameObject("camera 1");
	camera.addComponent<Hzn::CameraComponent>();
	camera.addComponent<Hzn::TransformComponent>();

	object0.addChild(camera);
	object0.addChild(object1);


	HZN_INFO(object1.getPrevSibling().getComponent<Hzn::NameComponent>().m_Name);
	HZN_INFO(camera.getNextSibling().getComponent<Hzn::NameComponent>().m_Name);


	/*Hzn::GameObject object0 = m_Scene->getGameObject("square 1");*/
	auto list = object0.getChildren();

	for (const auto& x : list)
	{
		HZN_INFO(x.getComponent<Hzn::NameComponent>().m_Name);
	}
}

void EditorLayer::onDetach()
{
	Hzn::SceneManager::close("scenes/custom_scene.json");
}

void EditorLayer::onUpdate(Hzn::TimeStep ts)
{
	if (m_ViewportFocused && m_ViewportHovered && !m_PlayMode) {
		m_EditorCameraController.onUpdate(ts);
	}

	m_FrameBuffer->bind();

	// here, in case if the framebuffer is re-created, and the last known
	// viewport size does not match the viewport size of the new framebuffer, then
	// we update all the camera components to the proper aspect ratio, and update the last known viewport size.
	auto& props = m_FrameBuffer->getProps();

	if (m_Scene) {
		lastViewportSize = m_Scene->onViewportResize(props.width, props.height);
		m_EditorCameraController.getCamera().setAspectRatio(lastViewportSize.x / lastViewportSize.y);
	}


	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	/*Hzn::Renderer2D::beginScene(m_CameraController.getCamera());*/
	// update the scene.
	if (m_Scene) {
		if (m_PlayMode)
			m_Scene->onUpdate(ts);
		else
			m_Scene->onEditorUpdate(m_EditorCameraController.getCamera(), ts);
	}
	// unbind the current framebuffer.
	/*Hzn::Renderer2D::endScene();*/

	m_FrameBuffer->unbind();
}

void EditorLayer::onEvent(Hzn::Event& e)
{
	if (m_ViewportFocused && m_ViewportHovered && !m_PlayMode) {
		m_EditorCameraController.onEvent(e);
	}
}

void EditorLayer::onRenderImgui()
{
	/*static bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo)*/;

	auto& window = Hzn::App::getApp().getAppWindow();
	auto stats = Hzn::Renderer2D::getStats();

	// imgui code reference: https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp

	// DOCKING CODE.

	// docking options.
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	// DOCKING BEGIN.
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	// NEW PROJECT MODAL BEGIN.

	/*if (ImGui::Button("Delete.."))
		ImGui::OpenPopup("New Project?");*/
	// Always center this window when appearing
	// NEW PROJECT MODAL END.

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			//ImGui::MenuItem("Padding", NULL, &opt_padding);
			//ImGui::Separator();

			//if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			//if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			//if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			//if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			//if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			//ImGui::Separator();

			if(ImGui::MenuItem("New Project"))
			{
				request_NewProject = true;
			}

			//if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O", false))
			//{
			//	std::string projectFolderPath = Hzn::FileDialogs::openFolder();
			//	projectRootFolder = std::filesystem::path(projectFolderPath).string();

			//	//Check if the dtring returns empty or not
			//	/*if (projectFolderPath != "") {
			//		projectRootFolder = std::filesystem::path(projectFolderPath).string();
			//		projectPath = "Project(" + projectRootFolder + ")";
			//	}*/

			//	//set assets path of current project
			//	assetPath = projectRootFolder + "\\assets";
			//	m_CurrentDirectory = assetPath;

			//	std::map<std::string, std::string> spriteFormat;

			//	//create texture for image file and sprites from sprite sheets
			//	for (const auto& entry : std::filesystem::recursive_directory_iterator(assetPath))
			//	{

			//		if (!entry.is_directory() && entry.path().string().find("DirectoryIcon.png") != std::string::npos) {
			//			folderIcon = Hzn::Texture2D::create(entry.path().string());
			//		}

			//		if (!entry.is_directory() && entry.path().string().find("FileIcon.png") != std::string::npos) {

			//			fileIcon = Hzn::Texture2D::create(entry.path().string());
			//		}

			//		/*if (!entry.is_directory() && entry.path().parent_path().string().find("audios") != std::string::npos)
			//		{
			//			assetManager.LoadAudio(entry.path().string(), entry.path().string());
			//		}*/


			//		if (!entry.is_directory() && entry.path().string().find(".png") != std::string::npos) {
			//			assetManager.LoadTexture(entry.path().string(), entry.path().string());
			//		}

			//		if (!entry.is_directory() && entry.path().parent_path().string().find("sprites") != std::string::npos && entry.path().string().find(".png") != std::string::npos) {

			//			for (const auto& metaFile : std::filesystem::recursive_directory_iterator(entry.path().parent_path())) {

			//				if (metaFile.path().string().find(".meta") != std::string::npos && metaFile.path().filename().string().substr(0, metaFile.path().filename().string().find(".")) == entry.path().filename().string().substr(0, entry.path().filename().string().find("."))) {
			//					std::ifstream infile(metaFile.path().c_str(), std::ifstream::binary);
			//					std::string line;

			//					while (std::getline(infile, line)) {
			//						std::istringstream is_line(line);
			//						std::string key;
			//						if (std::getline(is_line, key, ':'))
			//						{
			//							std::string value;

			//							if (std::getline(is_line, value))
			//							{
			//								spriteFormat[key] = value;
			//							}
			//						}
			//					}

			//				}
			//			}

			//			auto spriteSheet = Hzn::Texture2D::create(entry.path().string());


			//			for (size_t i = 0; i < std::stoi(spriteFormat.find("row")->second); i++)
			//			{
			//				for (size_t j = 0; j < std::stoi(spriteFormat.find("column")->second); j++)
			//				{
			//					std::string currentSprite = "(" + std::to_string(i) + "," + std::to_string(j) + ")";
			//					spriteMap.insert(std::make_pair(entry.path().string().append(currentSprite), Hzn::Sprite2D::create(spriteSheet, { i, j }, { std::stof(spriteFormat.find("width")->second),std::stof(spriteFormat.find("height")->second) })));
			//				}
			//			}




			//		}
			//	}

			//}

			//if (ImGui::MenuItem("Save scene"))
			//{
			//	Hzn::SceneManager::save(currentScenePath);
			//}

			if (ImGui::MenuItem("Play"))
			{
				m_PlayMode = !m_PlayMode;
			}

            if (ImGui::MenuItem("Exit"))
            {
	            Hzn::App::getApp().close();
            }



			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (request_NewProject)
		ImGui::OpenPopup("New Project");

	if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			request_NewProject = false;
		}
		ImGui::EndPopup();
	}

	// OBJECT HIERARCHY BEGIN
	drawHierarchy();
	// OBJECT HIERARCHY END

	/*static bool show = true;*/
	// SETTINGS BEGIN.
	ImGui::Begin("Components");
	if (selectedObjectId != std::numeric_limits<uint32_t>::max()) {
		auto selectedObj = m_Scene->getGameObject(selectedObjectId);
		Hzn::displayIfExists<Hzn::NameComponent>(selectedObj);
		Hzn::displayIfExists<Hzn::TransformComponent>(selectedObj);
		Hzn::displayIfExists<Hzn::RenderComponent>(selectedObj);
		Hzn::displayIfExists<Hzn::CameraComponent>(selectedObj);
	}
	ImGui::End();
	// SETTINGS END.

	//content browser begin

	//projectContextObject = m_CurrentDirectory.string();
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
				icon = assetManager.GetTexture(entry.path().string());
			}

			else
			{
				icon = fileIcon;
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			if (entry.path().parent_path().string().find("sprites") != std::string::npos)
			{

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

				const wchar_t* filename = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", filename, (wcslen(filename) + 1) * sizeof(wchar_t));
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

	//content browser end

	// VIEWPORT BEGIN
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");

	// get the states of the viewport.
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	// ImGui layer will not block the events if the viewport is focused and hovered.
	Hzn::App::getApp().getImguiLayer()->blockEvents(!m_ViewportFocused || !m_ViewportHovered);

	// if viewport size changes then we re-create the frame buffer.
	glm::vec2 viewportSize = *reinterpret_cast<glm::vec2*>(&(ImGui::GetContentRegionAvail()));
	if (lastViewportSize != viewportSize)
	{
		m_FrameBuffer->recreate(viewportSize.x, viewportSize.y);
	}

	/*HZN_INFO("{0}, {1}", viewportSize.x, viewportSize.y);*/

	ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer->getColorAttachmentId()),
		{ viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* filepath = (const wchar_t*)payload->Data;

			std::wstring ws(filepath);

			std::string str(ws.begin(), ws.end());

			openScene(str);

		}
		ImGui::EndDragDropTarget();
	}


	ImGui::End();
	ImGui::PopStyleVar();
	// VIEWPORT END.

	ImGui::End();
	// DOCKING END.
}

void EditorLayer::drawHierarchy()
{
	ImGui::Begin("Object Hierarchy");
	auto list = m_Scene->getAllRootIds();

	/*openHierarchyPopup = false;*/
	openHierarchyPopup = ImGui::IsPopupOpen("HierarchyObjectPopup");

	for (const auto& x : list)
	{
		drawObjects(m_Scene->getGameObject(x));
	}

	if (openHierarchyPopup) {
		if (ImGui::IsPopupOpen("HierarchyObjectPopup")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::OpenPopup("HierarchyObjectPopup");

		if (ImGui::BeginPopup("HierarchyObjectPopup")) {
			if (ImGui::MenuItem("Copy", NULL, false)) {
				/*copiedGameObject = m_Scene->getGameObject(selectedObject);*/
			}
			if (ImGui::MenuItem("Paste", NULL, false)) {
				// Do stuff here 
			}
			if (ImGui::MenuItem("Duplicate", NULL, false)) {
				// Do stuff here 
			}
			if (ImGui::MenuItem("Delete", NULL, false)) {
				Hzn::GameObject obj = m_Scene->getGameObject(selectedObjectId);
				m_Scene->destroyGameObject(obj);

				selectedObject = std::string();
				selectedObjectId = std::numeric_limits<uint32_t>::max();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Create Empty", NULL, false)) {
				// Do stuff here
				Hzn::GameObject newObject = m_Scene->createGameObject("Game Object");
				m_Scene->getGameObject(selectedObjectId).addChild(newObject);
				newObject.addComponent<Hzn::TransformComponent>();
			}

			ImGui::EndPopup();
		}
	}

	// Right-click
	ImVec2 emptySpaceSize = ImGui::GetContentRegionAvail();
	if (emptySpaceSize.x < 50) emptySpaceSize.x = 50;
	if (emptySpaceSize.y < 50) emptySpaceSize.y = 50;
	ImGui::InvisibleButton("canvas", emptySpaceSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PAYLOAD")) {
			Hzn::GameObject receivedObject = m_Scene->getGameObject((uint32_t) * (const int*)payload->Data);
			receivedObject.setParent(Hzn::GameObject());
		}

		ImGui::EndDragDropTarget();
	}

	// Context menu (under default mouse threshold)
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
		ImGui::OpenPopupOnItemClick("contextHierarchy", ImGuiPopupFlags_MouseButtonRight);
	}
	if (ImGui::BeginPopup("contextHierarchy")) {
		selectedObject = "";
		selectedObjectId = std::numeric_limits<uint32_t>::max();

		if (ImGui::MenuItem("Create Empty", NULL, false)) {
			Hzn::GameObject newObject = m_Scene->createGameObject("Game Object");
			newObject.addComponent<Hzn::TransformComponent>();
		}

		ImGui::EndPopup();
	}
	// Left click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		selectedObject = "";
		selectedObjectId = std::numeric_limits<uint32_t>::max();
	}

	ImGui::End();
}

void EditorLayer::drawObjects(Hzn::GameObject& object)
{
	std::vector<Hzn::GameObject> list = object.getChildren();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

	if (list.size() == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	if (selectedObjectId == object.getObjectId()) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	auto& nameComponent = object.getComponent<Hzn::NameComponent>();

	bool open = ImGui::TreeNodeEx(nameComponent.m_Name.c_str(), flags);
	
	// Drag and drop
	ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover; // | ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

	if (ImGui::BeginDragDropSource(src_flags)) {
		int id = object.getObjectId();

		ImGui::SetDragDropPayload("HIERARCHY_PAYLOAD", &id, sizeof(int));
		ImGui::Text(nameComponent.m_Name.c_str());
		ImGui::EndDragDropSource();

		std::vector<std::string> names = m_Scene->allGameObjectNames();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PAYLOAD")) {
			Hzn::GameObject receivedObject = m_Scene->getGameObject((uint32_t) * (const int*)payload->Data);

			// Set new parent only if the target object is not a child of the source object
			receivedObject.setParent(object);
			/*std::vector<Hzn::GameObject> children = receivedObject.getChildrenAll();
			if (!std::count(children.begin(), children.end(), object)) {
				if (receivedObject.getComponent<Hzn::RelationComponent>().hasParent()) {
					receivedObject.getParent().removeChild(receivedObject);
				}
				object.addChild(receivedObject);
			}*/
		}

		ImGui::EndDragDropTarget();
	}

	// Left click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		selectedObjectId = object.getObjectId();
	}

	// Right click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		selectedObjectId = object.getObjectId();

		ImGui::OpenPopup("HierarchyObjectPopup");
	}

	openHierarchyPopup |= ImGui::IsPopupOpen("HierarchyObjectPopup");

	if (open) {
		for (auto& x : list)
		{
			drawObjects(x);
		}
	}

	if (open && list.size() > 0) {
		ImGui::TreePop();
	}
}



void EditorLayer::openScene(const std::string path)
{
	m_Scene = Hzn::SceneManager::load(path);
	currentScenePath = path;
}