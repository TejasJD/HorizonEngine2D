#include <pch.h>
#include "EditorLayer.h"
#include "GraphEditor.h"
#include "Modals.h"
#include "ContentBrowser.h"
#include "ComponentDisplays.h"

std::shared_ptr<Hzn::Scene> EditorData::s_Scene_Active;
std::shared_ptr<Hzn::Project> EditorData::m_Project_Active;
std::string ContentBrowser::m_CurrentTexturePath;

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
		Hzn::FrameBufferTextureFormat::RED_INTEGER,
		Hzn::FrameBufferTextureFormat::DEPTH24_STENCIL8
	};

	m_FrameBuffer = Hzn::FrameBuffer::create(props);
	Hzn::ScriptEngine::init();
}

void EditorLayer::onDetach()
{
	Hzn::ProjectManager::close();
	Hzn::ScriptEngine::destroy();
}

void EditorLayer::onUpdate(Hzn::TimeStep ts)
{
	m_Ts = ts;
	// input.
	if (Hzn::Input::keyPressed(Hzn::Key::LeftControl) || Hzn::Input::keyPressed(Hzn::Key::RightControl)) m_CtrlPressed = true;
	else m_CtrlPressed = false;

	if (Hzn::SceneManager::isOpen())
	{
		m_FrameBuffer->bind();
		// here, in case if the framebuffer is re-created, and the last known
		// viewport size does not match the viewport size of the new framebuffer, then
		// we update all the camera components to the proper aspect ratio, and update the last known viewport size.
		auto& props = m_FrameBuffer->getProps();
		if (Hzn::SceneManager::isOpen()) {
			lastViewportSize = EditorData::s_Scene_Active->onViewportResize(props.width, props.height);
			m_EditorCameraController.getCamera().setAspectRatio(lastViewportSize.x / lastViewportSize.y);
		}

		Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hzn::RenderCall::submitClear();

		// clear frame buffer texture attachment that has entity to -1
		m_FrameBuffer->clearColorAttachment(1, -1);

		if (m_ViewportFocused && m_ViewportHovered &&
			Hzn::SceneManager::isOpen() &&
			Hzn::SceneManager::getSceneState() == Hzn::SceneState::Edit)
		{
			m_EditorCameraController.onUpdate(ts);
		}

		Hzn::SceneManager::update(m_EditorCameraController.getCamera(), ts);
		// checking if the mouse pointer is hovering on the viewport, and retrieving the right position.
		auto mousePos = ImGui::GetMousePos();
		mousePos.x -= m_ViewportBounds[0].x;
		mousePos.y -= m_ViewportBounds[0].y;
		/*mousePos.y = m_ViewportBounds[0].y - mousePos.y;*/
		auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		mousePos.y = viewportSize.y - mousePos.y;
		if (0 < mousePos.x && mousePos.x < viewportSize.x && 0 < mousePos.y && mousePos.y < viewportSize.y)
		{
			m_HoveredObjectId = m_FrameBuffer->readPixel(1, mousePos.x, mousePos.y);
		}
		// unbind the current framebuffer.
		m_FrameBuffer->unbind();
	}
}

void EditorLayer::onEvent(Hzn::Event& e)
{
	Hzn::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hzn::MouseButtonPressedEvent>(std::bind(&EditorLayer::onMouseButtonPressed, this, std::placeholders::_1));
	dispatcher.Dispatch<Hzn::KeyPressedEvent>(std::bind(&EditorLayer::onKeyPressed, this, std::placeholders::_1));
	if (m_ViewportFocused && m_ViewportHovered)
	{
		m_EditorCameraController.onEvent(e);
	}
}


void EditorLayer::onRenderImgui()
{
	/*HZN_INFO(m_SelectedObjectId);*/
	/*static bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo);*/

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
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	// STATS BEGIN
	/*ImGui::Begin("stats");
	ImGui::Text("Frame time: %.3f", m_Ts);
	ImGui::End();*/
	// STATS END


	//End Docking here
	style.WindowMinSize.x = minWinSizeX;
	// MENU BAR BEGIN
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
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

			if (ImGui::MenuItem("New Project"))
			{
				Modals::request_NewProject = true;
			}

			if (ImGui::MenuItem("Open Project"))
			{
				std::string str = Hzn::FileDialogs::openFile();
				if (!str.empty()) {
					EditorData::m_Project_Active = Hzn::ProjectManager::open(str);
					EditorData::s_Scene_Active = EditorData::m_Project_Active->getActiveScene();
					Modals::openProject();
					m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
					m_HoveredObjectId = -1;
				}
			}

			if (EditorData::m_Project_Active)
			{
				if (ImGui::MenuItem("Close Project"))
				{
					m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
					m_HoveredObjectId = -1;
					EditorData::s_Scene_Active.reset();
					EditorData::m_Project_Active.reset();
					Modals::projectRootFolder.clear();
					Hzn::ProjectManager::close();
				}

			}

			//if (ImGui::MenuItem("Save scene"))
			//{
			//	Hzn::SceneManager::save(currentScenePath);
			//}

			if (EditorData::m_Project_Active)
			{
				if (ImGui::MenuItem("New Scene"))
				{
					Modals::request_NewScene = true;
				}

				if (Hzn::SceneManager::isOpen())
				{
					if (Hzn::SceneManager::getSceneState() == Hzn::SceneState::Play)
					{
						if (ImGui::MenuItem("Stop")) Hzn::SceneManager::stop();
					}
					else
					{
						if (ImGui::MenuItem("Play")) Hzn::SceneManager::play();
					}
				}
			}

			if (ImGui::MenuItem("Exit"))
			{
				if (Hzn::ProjectManager::close())
				{
					m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
					m_HoveredObjectId = -1;
				}
				Hzn::App::getApp().close();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::MenuItem("Reload"))
			{
				Hzn::ScriptEngine::ReloadAssembly();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	// MENU BAR END

	//MODALS BEGIN
	//Scene pop-ups
	Modals::getCenterWindow();
	//new project
	{
		bool newProjectOpen = Modals::getNewProJPopup();
		if (newProjectOpen)
		{
			m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
			m_HoveredObjectId = -1;
		}
	}
	//New Scene
	{
		bool newSceneOpen = Modals::getNewScenePopup();
		if (newSceneOpen)
		{
			m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
			m_HoveredObjectId = -1;
		}
	}
	// MODALS END

	// OBJECT HIERARCHY BEGIN
	drawHierarchy();
	// OBJECT HIERARCHY END

	/*static bool show = true;*/
	// COMPONENTS BEGIN.
	ImGui::Begin(ICON_FA_GAMEPAD  " Components");
	if (EditorData::s_Scene_Active) {
		if (m_SelectedObjectId != std::numeric_limits<uint32_t>::max()) {
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
			ImGui::BeginChild("ExistingComponents", ImVec2{ 0, 400 }, true, window_flags);
			auto selectedObj = EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId);
			Hzn::ComponentDisplays::displayIfExists(selectedObj, Hzn::AllComponents{});
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
		auto cursorPos = ImGui::GetCursorPosX();
		auto width = ImGui::GetWindowWidth();
		static bool showComponentMenu = false;
		if (m_SelectedObjectId != std::numeric_limits<uint32_t>::max())
		{
			auto val = ImGui::CalcTextSize("Add Component");
			/*ImGui::SetCursorPosX((width - (val.x + 60)) * 0.5f);*/
			if (showComponentMenu |= ImGui::Button("Add Component", ImVec2{ -FLT_MIN, val.y + 20 }))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
				ImGui::BeginChild("ChildR", ImVec2(0, 250), true, window_flags);

				auto list = Hzn::getComponentStringList(Hzn::SelectableComponents{});
				for (auto& it : list)
				{
					std::string_view val = std::string_view(it);
					auto hzn = val.find("Hzn");
					std::string componentName = std::string(val.substr(hzn + 5));
					if (ImGui::Button(componentName.c_str(), ImVec2(-FLT_MIN, 0)))
					{
						Hzn::addComponent(EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId), std::string(it));
					}
				}

				ImGui::EndChild();
				ImGui::PopStyleVar();

				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					showComponentMenu = false;
				}
			}
		}
	}
	ImGui::End();
	// COMPONENTS END.

	//CONTENT BROWSER BEGIN
	if (EditorData::m_Project_Active)
	{
		ContentBrowser(EditorData::m_Project_Active->getPath().parent_path().string());
	}
	// CONTENT BROWSER END

	//Sprites BEGIN
	ImGui::Begin("Sprites");
	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;
	ImGui::Columns(columnCount, 0, false);
	int count = 0;
	if (!ContentBrowser::m_CurrentTexturePath.empty())
	{

		if (!Hzn::AssetManager::spriteStorage.empty())
		{

			for (auto sprite = Hzn::AssetManager::spriteStorage.begin(); sprite != Hzn::AssetManager::spriteStorage.end(); sprite++)
			{

				if (sprite->first.find(ContentBrowser::m_CurrentTexturePath) == std::string::npos)
				{
					continue;
				}

				std::string::size_type nPos1 = sprite->first.find_last_of(";");
				std::string::size_type nPos2 = sprite->first.find_last_of(";", nPos1 - 1);

				std::string spriteY = sprite->first.substr(nPos1 + 1);
				std::string spriteX = sprite->first.substr(nPos2 + 1, nPos1 - nPos2 - 1);

				std::string spriteTexCoords = "(" + std::to_string(std::stoi(spriteX)) + "," + std::to_string(std::stoi(spriteY)) + ")";

				ImGui::PushID(spriteTexCoords.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

				ImGui::ImageButton((ImTextureID)sprite->second->getSpriteSheet()->getId(), { thumbnailSize, thumbnailSize }, { sprite->second->getTexCoords()[0].x, sprite->second->getTexCoords()[2].y }, { sprite->second->getTexCoords()[2].x, sprite->second->getTexCoords()[0].y });
				ImGui::PopStyleColor();

				if (ImGui::BeginDragDropSource()) {
					std::filesystem::path currentSpritePath = ContentBrowser::m_CurrentTexturePath + "-;" + spriteX + ";" + spriteY;

					const wchar_t* filename = currentSpritePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM_SPRITE", filename, (wcslen(filename) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}



				ImGui::TextWrapped(spriteTexCoords.c_str());
				ImGui::NextColumn();
				ImGui::PopID();
			}
		}
	}

	ImGui::End();
	//Sprites END

	//std::vector<std::pair<int, int>> links;
	//// elsewhere in the code...
	//for (int i = 0; i < links.size(); ++i)
	//{
	//	const std::pair<int, int> p = links[i];
	//	// in this case, we just use the array index of the link
	//	// as the unique identifier
	//	ImNodes::Link(i, p.first, p.second);
	//}
	//ImGui::Begin("Node Editor");
	//ImNodes::BeginNodeEditor();

	//ImNodes::BeginNode(1);
	//ImNodes::BeginNodeTitleBar();
	//ImGui::TextUnformatted("Start Game Event");
	//ImNodes::EndNodeTitleBar();
	//ImNodes::BeginOutputAttribute(1);
	//ImGui::Text("Output Pin");
	//ImNodes::EndOutputAttribute();
	//ImNodes::EndNode();

	//ImNodes::BeginNode(2);
	//ImNodes::BeginNodeTitleBar();
	//ImGui::TextUnformatted("Keyboard Event");
	//ImNodes::EndNodeTitleBar();
	//ImNodes::BeginInputAttribute(1);
	//ImGui::Text("Input Pin");
	//ImNodes::EndInputAttribute();
	//ImNodes::BeginOutputAttribute(2);
	//ImGui::Text("Output Pin");
	//ImNodes::EndOutputAttribute();
	//ImNodes::EndNode();

	//ImNodes::BeginNode(3);
	//ImNodes::BeginNodeTitleBar();
	//ImGui::TextUnformatted("Mouse Event");
	//ImNodes::EndNodeTitleBar();
	///*

	//*/
	//ImNodes::EndNode();

	//ImNodes::BeginNode(4);
	//ImNodes::BeginNodeTitleBar();
	//ImGui::TextUnformatted("If");
	//ImNodes::EndNodeTitleBar();
	///*

	//*/
	//ImNodes::EndNode();



	//ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
	//ImNodes::EndNodeEditor();
	//int start_attr, end_attr;
	//if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
	//{
	//	links.push_back(std::make_pair(start_attr, end_attr));
	//}
	//ImGui::End();
	static std::vector<std::pair<int, int>> links;
	// NODE EDITOR BEGIN
	ImGui::Begin("Node Editor");

	/*ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);*/
	ImNodes::BeginNodeEditor();
	int hardcoded_node_id = 1;
	ImNodes::BeginNode(hardcoded_node_id);

	// Node Titlebar.
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.89f, 1.0f, 0.1f, 1.0f });
	ImNodes::BeginNodeTitleBar();
	ImGui::Text(ICON_FA_DIRECTIONS " Switch");
	ImNodes::EndNodeTitleBar();
	ImGui::PopStyleColor();

	// Data pins style
	const int input_attr_id = 1;
	ImNodes::BeginInputAttribute(input_attr_id, ImNodesPinShape_CircleFilled);
	ImGui::Text(ICON_FA_ARROW_RIGHT " In");
	ImNodes::EndInputAttribute();

	ImGui::SameLine();

	const int output_attr_id = 2;
	ImNodes::BeginOutputAttribute(output_attr_id, ImNodesPinShape_CircleFilled);
	ImGui::Text("Out " ICON_FA_ARROW_RIGHT);
	ImNodes::EndOutputAttribute();
	ImNodes::EndNode();

	ImNodes::BeginNode(2);

	// Node Titlebar.
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.89f, 1.0f, 0.1f, 1.0f });
	ImNodes::BeginNodeTitleBar();
	ImGui::Text(ICON_FA_DIRECTIONS " Switch");
	ImNodes::EndNodeTitleBar();
	ImGui::PopStyleColor();

	// Data pins style
	ImNodes::BeginInputAttribute(3, ImNodesPinShape_CircleFilled);
	ImGui::Text(ICON_FA_ARROW_RIGHT " In");
	ImNodes::EndInputAttribute();
	ImGui::SameLine();

	ImNodes::BeginOutputAttribute(4, ImNodesPinShape_CircleFilled);
	ImGui::Text("Out " ICON_FA_ARROW_RIGHT);
	ImNodes::EndOutputAttribute();
	ImNodes::EndNode();

	// elsewhere in the code...
	for (int i = 0; i < links.size(); ++i)
	{
		const std::pair<int, int> p = links[i];
		// in this case, we just use the array index of the link
		// as the unique identifier
		ImNodes::Link(i, p.first, p.second);
	}

	ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);

	ImNodes::EndNodeEditor();

	int start_attr = INT_MAX, end_attr = INT_MAX;
	if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
	{
		links.push_back(std::make_pair(start_attr, end_attr));
	}
	
	ImGui::End();
	// NODE EDITOR END

	// VIEWPORT BEGIN
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");

	/*HZN_DEBUG("{0}, {1}", cursorPos.x, cursorPos.y);*/

	// get the states of the viewport.
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	// ImGui layer will not block the events if the viewport is focused and hovered.
	Hzn::App::getApp().getImguiLayer()->blockEvents(!m_ViewportFocused && !m_ViewportHovered);

	// if viewport size changes then we re-create the frame buffer.
	if (Hzn::SceneManager::isOpen())
	{
		glm::vec2 viewportSize = *reinterpret_cast<glm::vec2*>(&(ImGui::GetContentRegionAvail()));
		if (lastViewportSize != viewportSize)
		{
			/*HZN_DEBUG("{}, {}", viewportSize.x, viewportSize.y);*/
			m_FrameBuffer->recreate(viewportSize.x, viewportSize.y);
		}
		/*HZN_INFO("{0}, {1}", viewportSize.x, viewportSize.y);*/

		ImGui::Image((ImTextureID)(uint64_t)m_FrameBuffer->getColorAttachmentId(),
			{ viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
	}

	// calculate the minimum and the maximum bounds for the viewport.
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();

	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* filepath = (const wchar_t*)payload->Data;

			std::wstring ws(filepath);

			openScene(std::filesystem::path(ws));

		}
		ImGui::EndDragDropTarget();
	}

	if (Hzn::SceneManager::isOpen() && m_SelectedObjectId != std::numeric_limits<uint32_t>::max())
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		auto sz = m_ViewportBounds[1] - m_ViewportBounds[0];
		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, sz.x, sz.y);

		auto selectedObj = EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId);
		auto& transformComponent = selectedObj.getComponent<Hzn::TransformComponent>();
		auto transform = selectedObj.getTransform();

		if (Hzn::SceneManager::getSceneState() == Hzn::SceneState::Edit)
		{
			ImGuizmo::Manipulate(
				glm::value_ptr(m_EditorCameraController.getCamera().getViewMatrix()),
				glm::value_ptr(m_EditorCameraController.getCamera().getProjectionMatrix()),
				m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));
		}
		else
		{
			Hzn::GameObject obj;
			if (obj = EditorData::s_Scene_Active->getActiveCamera())
			{
				auto& cameraComponent = obj.getComponent<Hzn::CameraComponent>();
				ImGuizmo::Manipulate(
					glm::value_ptr(glm::inverse(obj.getTransform())),
					glm::value_ptr(cameraComponent.m_Camera.getProjectionMatrix()),
					m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));
			}
		}

		if (ImGuizmo::IsUsing())
		{
			if (selectedObj.getParent())
			{
				transform = glm::inverse(selectedObj.getParent().getTransform()) * transform;
			}

			glm::vec3 translation = glm::vec3(0.0f);
			glm::quat orientation = glm::quat();
			glm::vec3 scale = glm::vec3(0.0f);
			glm::vec3 skew = glm::vec3(0.0f);
			glm::vec4 perspective = glm::vec4(0.0f);
			glm::decompose(transform, scale, orientation, translation, skew, perspective);

			glm::vec3 rotation = glm::eulerAngles(orientation);
			rotation = glm::degrees(rotation);

			transformComponent.m_Translation = translation;
			transformComponent.m_Rotation = rotation;
			transformComponent.m_Scale = scale;
		}
	}


	ImGui::End();
	ImGui::PopStyleVar();
	// VIEWPORT END.

	ImGui::End();
	// DOCKING END.
}

void EditorLayer::drawHierarchy()
{
	ImGui::Begin(ICON_FA_DICE_D6 " Object Hierarchy");
	if (Hzn::SceneManager::isOpen()) {
		auto list = EditorData::s_Scene_Active->getAllRootIds();

		/*openHierarchyPopup = false;*/
		openHierarchyPopup = ImGui::IsPopupOpen("HierarchyObjectPopup");

		for (const auto& x : list)
		{
			drawObjects(EditorData::s_Scene_Active->getGameObjectById(x));
		}

		if (openHierarchyPopup) {
			if (ImGui::IsPopupOpen("HierarchyObjectPopup")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::OpenPopup("HierarchyObjectPopup");

			if (ImGui::BeginPopup("HierarchyObjectPopup")) {
				if (ImGui::MenuItem("Copy", "Ctrl + C", false)) {
					copyObject();
				}
				if (ImGui::MenuItem("Paste", "Ctrl + V", false)) {
					pasteObject();
				}
				if (ImGui::MenuItem("Duplicate", "Ctrl + D", false)) {
					// Do stuff here
					duplicateObject();
				}
				if (ImGui::MenuItem("Delete", "Del", false)) {
					deleteObject();
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Create Empty", "Ctrl + N", false)) {
					createObject();
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
				Hzn::GameObject receivedObject = EditorData::s_Scene_Active->getGameObjectById((uint32_t) * (const int*)payload->Data);
				HZN_INFO(receivedObject.getObjectId());
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
			m_SelectedObjectId = std::numeric_limits<uint32_t>::max();

			if (ImGui::MenuItem("Create Empty", "Ctrl + N", false)) {
				Hzn::GameObject newObject = EditorData::s_Scene_Active->createGameObject("Game Object");
			}

			ImGui::EndPopup();
		}
		// Left click
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			/*selectedObject = "";*/
			m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
		}
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

	if (m_SelectedObjectId == object.getObjectId()) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	auto& nameComponent = object.getComponent<Hzn::NameComponent>();
	std::string objectName = std::string(ICON_FA_DICE_D6) + " " + nameComponent.m_Name.c_str();
	if(flags & ImGuiTreeNodeFlags_Selected) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.89f, 1.0f, 0.1f, 1.0f });
	bool open = ImGui::TreeNodeEx((void*)(intptr_t)object.getObjectId(), flags, objectName.c_str());
	if(flags & ImGuiTreeNodeFlags_Selected) ImGui::PopStyleColor();

	// Drag and drop
	ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover; // | ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

	if (ImGui::BeginDragDropSource(src_flags)) {
		int id = object.getObjectId();
		HZN_ERROR(id);
		ImGui::SetDragDropPayload("HIERARCHY_PAYLOAD", &id, sizeof(int));
		ImGui::Text(nameComponent.m_Name.c_str());
		ImGui::EndDragDropSource();

	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PAYLOAD")) {
			Hzn::GameObject receivedObject = EditorData::s_Scene_Active->getGameObjectById((uint32_t) * (const int*)payload->Data);
			HZN_WARN(receivedObject.getObjectId());
			receivedObject.setParent(object);
		}

		ImGui::EndDragDropTarget();
	}

	// Left click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		m_SelectedObjectId = object.getObjectId();
	}

	// Right click
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		m_SelectedObjectId = object.getObjectId();

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

void EditorLayer::openScene(const std::filesystem::path& filepath)
{
	m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
	m_HoveredObjectId = -1;
	Hzn::ProjectManager::openScene(filepath);
	EditorData::s_Scene_Active = EditorData::m_Project_Active->getActiveScene();
}

bool EditorLayer::onKeyPressed(Hzn::KeyPressedEvent& e)
{
	auto keycode = e.GetKeyCode();

	switch (keycode)
	{
	case Hzn::Key::LeftControl:
	{
		m_CtrlPressed = true;
		break;
	}
	case Hzn::Key::C:
	{
		if (m_CtrlPressed)
		{
			copyObject();
			m_CtrlPressed = false;
		}
		break;
	}
	case Hzn::Key::V:
	{
		if (m_CtrlPressed)
		{
			pasteObject();
			m_CtrlPressed = false;
		}
		break;
	}
	case Hzn::Key::D:
	{
		if (m_CtrlPressed)
		{
			duplicateObject();
			m_CtrlPressed = false;
		}
		break;
	}
	case Hzn::Key::N:
	{
		if (m_CtrlPressed)
		{
			createObject();
			m_CtrlPressed = false;
		}
		break;
	}
	case Hzn::Key::Q:
	{
		m_GizmoType = ImGuizmo::OPERATION::NONE;
		m_CtrlPressed = false;
		break;
	}
	case Hzn::Key::W:
	{
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		m_CtrlPressed = false;
		break;
	}
	case Hzn::Key::E:
	{
		m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		m_CtrlPressed = false;
		break;
	}
	case Hzn::Key::R:
	{
		m_GizmoType = ImGuizmo::OPERATION::SCALE;
		m_CtrlPressed = false;
		break;
	}
	case Hzn::Key::Delete:
	{
		deleteObject();
		m_CtrlPressed = false;
		break;
	}
	default: 
	{
		m_CtrlPressed = false;
		break;
	}
	}

	return false;
}

bool EditorLayer::onMouseButtonPressed(Hzn::MouseButtonPressedEvent& e)
{
	auto mouseButton = e.GetMouseButton();

	if (mouseButton == Hzn::Mouse::ButtonLeft)
	{
		if (Hzn::SceneManager::isOpen())
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
			{
				m_SelectedObjectId = (uint32_t)m_HoveredObjectId;
			}
		}
	}

	return false;
}

void EditorLayer::copyObject()
{
	if (m_SelectedObjectId != std::numeric_limits<uint32_t>::max()) {
		m_CopiedObjectId = m_SelectedObjectId;
	}
}

void EditorLayer::pasteObject()
{
	if (m_CopiedObjectId != std::numeric_limits<uint32_t>::max() && EditorData::s_Scene_Active->getGameObjectById(m_CopiedObjectId)) {
		Hzn::GameObject obj = EditorData::s_Scene_Active->getGameObjectById(m_CopiedObjectId);
		if (obj.getParent())
		{
			obj.duplicateAsChild();
		}
		else obj.duplicate();
	}
}

void EditorLayer::duplicateObject()
{
	Hzn::GameObject obj = EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId);
	if (obj.getParent())
	{
		obj.duplicateAsChild();
	}
	else obj.duplicate();
}

void EditorLayer::createObject()
{
	Hzn::GameObject newObject = EditorData::s_Scene_Active->createGameObject("Game Object");

	if (m_SelectedObjectId != std::numeric_limits<uint32_t>::max())
	{
		Hzn::GameObject selectedObj = EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId);
		selectedObj.addChild(newObject);
		auto& transformComponent = newObject.getComponent<Hzn::TransformComponent>();
		auto transform = selectedObj.getTransform() * transformComponent.getModelMatrix();

		glm::vec3 translation = glm::vec3(0.0f);
		glm::quat orientation = glm::quat();
		glm::vec3 scale = glm::vec3(0.0f);
		glm::vec3 skew = glm::vec3(0.0f);
		glm::vec4 perspective = glm::vec4(0.0f);
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		glm::vec3 rotation = glm::eulerAngles(orientation);
		rotation = glm::degrees(rotation);

		transformComponent.m_Translation = translation;
		transformComponent.m_Rotation = rotation;
		transformComponent.m_Scale = scale;
	}
}

void EditorLayer::deleteObject()
{
	if (m_SelectedObjectId != std::numeric_limits<uint32_t>::max()) {
		Hzn::GameObject obj = EditorData::s_Scene_Active->getGameObjectById(m_SelectedObjectId);
		EditorData::s_Scene_Active->destroyGameObject(obj);
		m_HoveredObjectId = -1;
		m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
	}
}