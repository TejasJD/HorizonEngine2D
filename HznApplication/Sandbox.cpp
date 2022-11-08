#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"
#include "imgui_internal.h"

Sandbox::Sandbox()
	: m_AspectRatio(static_cast<float>(Hzn::App::getApp().getAppWindow().getWidth()) / Hzn::App::getApp().getAppWindow().getHeight()),
	m_CameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
{
	m_Bear = Hzn::Texture2D::create("assets/textures/bear.png");
	m_SpriteSheet = Hzn::Texture2D::create("assets/sample_game_assets/Tilemap/tilemap_packed.png");
	m_Sprite = Hzn::Sprite2D::create(m_SpriteSheet, { 0.0f, 0.0f }, { 16.0f, 16.0f }, { 1, 1 });
}

void Sandbox::onAttach()
{

}

void Sandbox::onDetach()
{

}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	const auto currentTime = static_cast<float>(glfwGetTime());
	m_FrameCounter++;
	if (currentTime - m_PreviousFrame >= 1.0f)
	{
		m_Fps = static_cast<const float>(m_FrameCounter);
		m_PreviousFrame = currentTime;
		m_FrameCounter = 0;
	}
	m_CameraController.onUpdate(deltaTime);
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(m_CameraController.getCamera()));
	// generate gradient across the grid.
	//for (int32_t i = 0; i < quads; ++i)
	//{
	//	for (int32_t j = 0; j < quads; ++j)
	//	{
	//		// colored quads.
	//		const glm::vec4 color = glm::lerp(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), (float)(i + j) / (2 * quads));
	//		Hzn::Renderer2D::drawQuad({ i * 0.11f, j * 0.11f, 0.0f }, quadAngle, glm::vec3(0.1f), color);
	//		// textured quads.
	//		/*Hzn::Renderer2D::drawQuad({ i * 1.11f, j * 1.11f, 1.0f }, quadAngle, glm::vec3(1.0f), someSky);*/
	//	}
	//}
	Hzn::Renderer2D::drawSprite({ 0.0f, 0.0f, 0.0f }, glm::vec3(1.0f), m_Sprite);

	Hzn::Renderer2D::endScene();
}

void Sandbox::onEvent(Hzn::Event& event)
{
	m_CameraController.onEvent(event);
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

void Sandbox::onRenderImgui()
{
	ImGui::Begin("FPS Counter");
	ImGui::Text("Frames: %.2f", m_Fps);
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::SliderInt("Grid Side", &quads, 5, 1000);
	ImGui::SliderFloat("Quad Angle:", &quadAngle, -180.0f, 180.0f);
	/*ImGui::Image((ImTextureID)m_Bear->getId(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });*/
	auto texCoords = m_Sprite->getTexCoords();
	ImGui::Image((ImTextureID)m_SpriteSheet->getId(), { 160.0f, 160.0f }, { texCoords[0].x, texCoords[2].y },
		{ texCoords[2].x, texCoords[0].y });
	ImGui::End();

	const Hzn::Renderer2DStats& stats = Hzn::Renderer2D::getStats();

	ImGui::Begin("Metrics");
	ImGui::Text("Draw calls: %d", stats.draws);
	ImGui::Text("Quads: %d", stats.quads);
	ImGui::Text("Vertices: %d", stats.vertices);
	ImGui::Text("Indices: %d", stats.indices);
	ImGui::End();

	// ***** NODE EDITOR *****

	
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
		links.push_back(NodeLink(0, 0, 2, 0));
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
	
	ImGui::End();
	




}

