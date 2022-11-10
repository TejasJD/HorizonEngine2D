#include "pch.h"
#include <HorizonEngine.h>
#include <HznEntryPoint.h>
#include "imgui_internal.h"
#include "Sandbox.h"
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<HznApp>();
	//app->addLayer(new EditorLayer());
	app->addLayer(new Sandbox());
	return app;
}

// ************************************************************************

// *********** EDITOR LAYER **********
EditorLayer::EditorLayer(const char* name) : Hzn::Layer(name) {
	
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

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

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

	// ********** NODE EDITOR **********
	ImGui::Begin("Node Editor");

	struct Node
	{
		int ID;
		char Name[32];
		ImVec2 Pos, Size; 
		float Value; 
		ImVec4 Color;
		int InputsCount, OutputsCount; 

		Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count) { ID = id; strcpy(Name, name); Pos = pos; Value = value; Color = color; InputsCount = inputs_count; OutputsCount = outputs_count; }

		ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
		ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
	};

	struct NodeLink
	{
		int InputIdx, InputSlot, OutputIdx, OutputSlot;

		NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
	};

	// State
	static ImVector<Node> nodes;
	static ImVector<NodeLink> links;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static bool inited = false;
	static bool show_grid = true;
	static int node_selected = -1;

	ImGuiIO& io = ImGui::GetIO();
	if (!inited)
	{
		nodes.push_back(Node(0, "Test Node 1", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
		nodes.push_back(Node(1, "Test Node 2", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
		nodes.push_back(Node(2, "Test Node 3", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));
		links.push_back(NodeLink(0, 0, 1, 0));
		links.push_back(NodeLink(1, 0, 2, 1));
		inited = true;
	}

	// Nodes on the LHS 

	bool open_context_menu = false;
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;
	ImGui::BeginChild("node_list", ImVec2(100, 0));
	ImGui::Text("List of Nodes");
	ImGui::Separator();
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		if (ImGui::Selectable(node->Name, node->ID == node_selected))
			node_selected = node->ID;
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_list = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginGroup();

	const float NODE_SLOT_RADIUS = 4.0f;
	const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

	// Child Canvas 
	ImGui::Text("Hold middle mouse button to scroll (%.2f, %.2f)", scrolling.x, scrolling.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show Grid", &show_grid);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleVar(); // WindowPadding
	ImGui::PushItemWidth(120.0f);

	const ImVec2 offset = ImGui::GetCursorScreenPos() + scrolling;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Grid 
	if (show_grid)
	{
		ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
		float GRID_SZ = 64.0f;
		ImVec2 win_pos = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetWindowSize();
		for (float x = fmodf(scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
			draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
		for (float y = fmodf(scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
			draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
	}

	// Display links
	draw_list->ChannelsSplit(2);
	draw_list->ChannelsSetCurrent(0); // Background
	for (int link_idx = 0; link_idx < links.Size; link_idx++)
	{
		NodeLink* link = &links[link_idx];
		Node* node_inp = &nodes[link->InputIdx];
		Node* node_out = &nodes[link->OutputIdx];
		ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
		ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
		draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 255), 3.0f);
	}

	// Nodes 
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		ImVec2 node_rect_min = offset + node->Pos;

		// Contents of Node 
		draw_list->ChannelsSetCurrent(1); // Foreground
		bool old_any_active = ImGui::IsAnyItemActive();
		ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
		ImGui::BeginGroup(); // Lock horizontal position
		ImGui::Text("%s", node->Name);
		/*
		ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
		ImGui::ColorEdit3("##color", &node->Color.x);
		*/
		ImGui::EndGroup();

		// Save the size of what we have emitted and whether any of the widgets are being used
		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
		node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
		ImVec2 node_rect_max = node_rect_min + node->Size;

		// Display node box
		draw_list->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(node_rect_min);
		ImGui::InvisibleButton("node", node->Size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			node_selected = node->ID;
		if (node_moving_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			node->Pos = node->Pos + io.MouseDelta;

		ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
		draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
		draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
		for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
		for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

		ImGui::PopID();
	};
	draw_list->ChannelsMerge();

	// Open context menu
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) || !ImGui::IsAnyItemHovered())
		{
			node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
			open_context_menu = true;
		}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
			ImGui::Text("Node '%s'", node->Name);
			ImGui::Separator();
			if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
			if (ImGui::MenuItem("Delete", NULL, false, false)) {}
			if (ImGui::MenuItem("Copy", NULL, false, false)) {}
		}
		else
		{
			if (ImGui::MenuItem("Add Event")) { nodes.push_back(Node(nodes.Size, "New Event", scene_pos, 0.5f, ImColor(100, 100, 200), 1, 1)); }
			if (ImGui::MenuItem("Add Variable")) { nodes.push_back(Node(nodes.Size, "New Variable", scene_pos, 0.5f, ImColor(100, 100, 200), 1, 1)); }
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f))
		scrolling = scrolling + io.MouseDelta;

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::EndGroup();

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
				std::string projectFilePath = Hzn::FileDialogs::openFile();

				//Check if the dtring returns empty or not
				if (projectFilePath != "") {
					projectRootFolder = std::filesystem::path(projectFilePath).parent_path().string();
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
		// (one of the two depending on the mouse m_Position)
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

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			contextObject = entry.path().string();
		}

		if (!entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			contextObject = entry.path().string();

			ImGui::OpenPopup("dirContextObject");
		}

		if (entryIsFile && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
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
		//HZN_CORE_DEBUG("Object: " + contextObject);

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
			
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Create new file", NULL, false)) {
			// Do stuff here 

			if (std::filesystem::exists(contextObject + "/new file"))
			{
				HZN_CRITICAL("new file already exists");
			}
			else {
				std::ofstream(contextObject + "/new file");
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
		

		if (ImGui::MenuItem("Create new file", NULL, false)) {
			if (std::filesystem::exists(projectRootFolder + "/new file"))
			{
				HZN_CRITICAL("new file already exists");
			}
			else {
				std::ofstream(projectRootFolder + "/new file");
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