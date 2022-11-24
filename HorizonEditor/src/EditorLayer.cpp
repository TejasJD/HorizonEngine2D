#include <pch.h>
#include "EditorLayer.h"
#include "GraphEditor.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "Modals.h"
#include "ContentBrowser.h"

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
		Hzn::FrameBufferTextureFormat::DEPTH24_STENCIL8
	};

	m_FrameBuffer = Hzn::FrameBuffer::create(props);
}

void EditorLayer::onDetach()
{
	Hzn::ProjectManager::close();
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

	if (EditorData::s_Scene_Active) {
		lastViewportSize = EditorData::s_Scene_Active->onViewportResize(props.width, props.height);
		m_EditorCameraController.getCamera().setAspectRatio(lastViewportSize.x / lastViewportSize.y);
	}


	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	/*Hzn::Renderer2D::beginScene(m_CameraController.getCamera());*/
	// update the scene.
	if (EditorData::s_Scene_Active) {
		if (m_PlayMode)
			EditorData::s_Scene_Active->onUpdate(ts);
		else
			EditorData::s_Scene_Active->onEditorUpdate(m_EditorCameraController.getCamera(), ts);
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






//// ***** CODE FOR NODE EDITOR *****
//template <typename T, std::size_t N>
//
//struct Array
//{
//	T data[N];
//	const size_t size() const { return N; }
//
//	const T operator [] (size_t index) const { return data[index]; }
//	operator T* () {
//		T* p = new T[N];
//		memcpy(p, data, sizeof(data));
//		return p;
//	}
//};
//
//template <typename T, typename ... U> Array(T, U...)->Array<T, 1 + sizeof...(U)>;
//
//struct GraphEditorDelegate : public GraphEditor::Delegate
//{
//	bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
//	{
//		return true;
//	}
//
//	void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
//	{
//		mNodes[nodeIndex].mSelected = selected;
//	}
//
//	void MoveSelectedNodes(const ImVec2 delta) override
//	{
//		for (auto& node : mNodes)
//		{
//			if (!node.mSelected)
//			{
//				continue;
//			}
//			node.x += delta.x;
//			node.y += delta.y;
//		}
//	}
//
//	virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
//	{
//	}
//
//	void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
//	{
//		mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
//	}
//
//	void DelLink(GraphEditor::LinkIndex linkIndex) override
//	{
//		mLinks.erase(mLinks.begin() + linkIndex);
//	}
//
//	void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override
//	{
//		drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
//		drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
//	}
//
//	const size_t GetTemplateCount() override
//	{
//		return sizeof(mTemplates) / sizeof(GraphEditor::Template);
//	}
//
//	const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
//	{
//		return mTemplates[index];
//	}
//
//	const size_t GetNodeCount() override
//	{
//		return mNodes.size();
//	}
//
//	const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
//	{
//		const auto& myNode = mNodes[index];
//		return GraphEditor::Node
//		{
//			myNode.name,
//			myNode.templateIndex,
//			ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
//			myNode.mSelected
//		};
//	}
//
//	const size_t GetLinkCount() override
//	{
//		return mLinks.size();
//	}
//
//	const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
//	{
//		return mLinks[index];
//	}
//
//	// Graph datas
//	static const inline GraphEditor::Template mTemplates[] = {
//		{
//			IM_COL32(160, 160, 180, 255),
//			IM_COL32(100, 100, 140, 255),
//			IM_COL32(110, 110, 150, 255),
//			1,
//			Array{"MyInput"},
//			nullptr,
//			2,
//			Array{"MyOutput0", "MyOuput1"},
//			nullptr
//		},
//
//		{
//			IM_COL32(180, 160, 160, 255),
//			IM_COL32(140, 100, 100, 255),
//			IM_COL32(150, 110, 110, 255),
//			3,
//			nullptr,
//			Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
//			1,
//			Array{"MyOutput0"},
//			Array{ IM_COL32(200,200,200,255)}
//		}
//	};
//
//	struct Node
//	{
//		const char* name;
//		GraphEditor::TemplateIndex templateIndex;
//		float x, y;
//		bool mSelected;
//	};
//
//	std::vector<Node> mNodes = {
//		{
//			"Start Node",
//			0,
//			0, 0,
//			false
//		},
//
//		{
//			"End Node",
//			0,
//			400, 0,
//			false
//		},
//
//		{
//			"My Node 2",
//			1,
//			400, 400,
//			false
//		}
//	};
//
//	std::vector<GraphEditor::Link> mLinks = { {0, 0, 1, 0} };
//};//End node editor





//			2,
//			Array{"MyOutput0", "MyOuput1"},
//			nullptr
//		},
//
//		{
//			IM_COL32(180, 160, 160, 255),
//			IM_COL32(140, 100, 100, 255),
//			IM_COL32(150, 110, 110, 255),
//			3,
//			nullptr,
//			Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
//			1,
//			Array{"MyOutput0"},
//			Array{ IM_COL32(200,200,200,255)}
//		}
//	};
//
//	struct Node
//	{
//		const char* name;
//		GraphEditor::TemplateIndex templateIndex;
//		float x, y;
//		bool mSelected;
//	};
//
//	std::vector<Node> mNodes = {
//		{
//			"Start Node",
//			0,
//			0, 0,
//			false
//		},
//
//		{
//			"End Node",
//			0,
//			400, 0,
//			false
//		},
//
//		{
//			"My Node 2",
//			1,
//			400, 400,
//			false
//		}
//	};
//
//	std::vector<GraphEditor::Link> mLinks = { {0, 0, 1, 0} };
//};

void EditorLayer::onRenderImgui()
{
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
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	//End Docking here


	// Options menu
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
				}

			}

			if (EditorData::m_Project_Active)
			{
				if (ImGui::MenuItem("Close Project"))
				{
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
			}

			if (EditorData::m_Project_Active)
			{
				if (ImGui::MenuItem("Play"))
				{
					m_PlayMode = !m_PlayMode;
				}
			}

			if (ImGui::MenuItem("Exit"))
			{
				Hzn::App::getApp().close();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}//End options

	//MODALS
	//Scene pop-ups
	Modals::getCenterWindow();
	//new project
	Modals::getNewProJPopup();
	//New Scene
	Modals::getNewScenePopup();





	// OBJECT HIERARCHY BEGIN
	drawHierarchy();
	// OBJECT HIERARCHY END

	/*static bool show = true;*/
	// SETTINGS BEGIN.
	ImGui::Begin("Components");
	if (EditorData::s_Scene_Active) {
		if (selectedObjectId != std::numeric_limits<uint32_t>::max()) {
			auto selectedObj = EditorData::s_Scene_Active->getGameObject(selectedObjectId);
			Hzn::displayIfExists<Hzn::NameComponent>(selectedObj);
			Hzn::displayIfExists<Hzn::TransformComponent>(selectedObj);
			Hzn::displayIfExists<Hzn::RenderComponent>(selectedObj);
			Hzn::displayIfExists<Hzn::CameraComponent>(selectedObj);
		}
	}
	ImGui::End();
	// SETTINGS END.

	//CONTENT BROWSER BEGIN
	ContentBrowser::ContentBrowser(Modals::projectRootFolder);
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

		if (Hzn::AssetManager::spriteStorage.size() > 0)
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

	//// VISUAL SCRIPTING BEGIN 
	//static GraphEditor::Options options;
	////static GraphEditorDelegate delegate;
	//static GraphEditor::ViewState viewState;
	//static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;
	//static bool showGraphEditor = true;

	//if (ImGui::CollapsingHeader("Graph Editor"))
	//{
	//	ImGui::Checkbox("Show GraphEditor", &showGraphEditor);
	//	GraphEditor::EditOptions(options);
	//}

	//if (showGraphEditor)
	//{
	//	ImGui::Begin("Graph Editor", NULL, 0);
	//	if (ImGui::Button("Fit all nodes"))
	//	{
	//		fit = GraphEditor::Fit_AllNodes;
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Fit selected nodes"))
	//	{
	//		fit = GraphEditor::Fit_SelectedNodes;
	//	}
	//	//GraphEditor::Show(delegate, options, viewState, true, &fit);

	//	ImGui::End();
	//}
	//// VISUAL SCRIPTING END 

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

			openScene(std::filesystem::path(str));

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
	if (EditorData::s_Scene_Active) {
		auto list = EditorData::s_Scene_Active->getAllRootIds();

		/*openHierarchyPopup = false;*/
		openHierarchyPopup = ImGui::IsPopupOpen("HierarchyObjectPopup");

		for (const auto& x : list)
		{
			drawObjects(EditorData::s_Scene_Active->getGameObject(x));
		}

		if (openHierarchyPopup) {
			if (ImGui::IsPopupOpen("HierarchyObjectPopup")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::OpenPopup("HierarchyObjectPopup");

			if (ImGui::BeginPopup("HierarchyObjectPopup")) {
				if (ImGui::MenuItem("Copy", NULL, false)) {
					/*copiedGameObject = EditorData::s_Scene_Active->getGameObject(selectedObject);*/
				}
				if (ImGui::MenuItem("Paste", NULL, false)) {
					// Do stuff here 
				}
				if (ImGui::MenuItem("Duplicate", NULL, false)) {
					// Do stuff here 
				}
				if (ImGui::MenuItem("Delete", NULL, false)) {
					Hzn::GameObject obj = EditorData::s_Scene_Active->getGameObject(selectedObjectId);
					EditorData::s_Scene_Active->destroyGameObject(obj);

					selectedObject = std::string();
					selectedObjectId = std::numeric_limits<uint32_t>::max();
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Create Empty", NULL, false)) {
					// Do stuff here
					Hzn::GameObject newObject = EditorData::s_Scene_Active->createGameObject("Game Object");
					EditorData::s_Scene_Active->getGameObject(selectedObjectId).addChild(newObject);
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
				Hzn::GameObject receivedObject = EditorData::s_Scene_Active->getGameObject((uint32_t) * (const int*)payload->Data);
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
				Hzn::GameObject newObject = EditorData::s_Scene_Active->createGameObject("Game Object");
				newObject.addComponent<Hzn::TransformComponent>();
			}

			ImGui::EndPopup();
		}
		// Left click
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			selectedObject = "";
			selectedObjectId = std::numeric_limits<uint32_t>::max();
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

		std::vector<std::string> names = EditorData::s_Scene_Active->allGameObjectNames();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PAYLOAD")) {
			Hzn::GameObject receivedObject = EditorData::s_Scene_Active->getGameObject((uint32_t) * (const int*)payload->Data);
			receivedObject.setParent(object);
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

void EditorLayer::openScene(const std::filesystem::path& filepath)
{
	Hzn::ProjectManager::openScene(filepath);
	EditorData::s_Scene_Active = EditorData::m_Project_Active->getActiveScene();
}
