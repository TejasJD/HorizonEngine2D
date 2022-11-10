#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "GraphEditor.h"


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
// ********** NODE EDITOR **********

template <typename T, std::size_t N>
struct Array
{
    T data[N];
    const size_t size() const { return N; }

    const T operator [] (size_t index) const { return data[index]; }
    operator T* () {
        T* p = new T[N];
        memcpy(p, data, sizeof(data));
        return p;
    }
};

template <typename T, typename ... U> Array(T, U...)->Array<T, 1 + sizeof...(U)>;

struct GraphEditorDelegate : public GraphEditor::Delegate
{
    bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
    {
        return true;
    }

    void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
    {
        mNodes[nodeIndex].mSelected = selected;
    }

    void MoveSelectedNodes(const ImVec2 delta) override
    {
        for (auto& node : mNodes)
        {
            if (!node.mSelected)
            {
                continue;
            }
            node.x += delta.x;
            node.y += delta.y;
        }
    }

    virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
    {
    }

    void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
    {
        mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
    }

    void DelLink(GraphEditor::LinkIndex linkIndex) override
    {
        mLinks.erase(mLinks.begin() + linkIndex);
    }

    void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override
    {
        drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
        drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
    }

    const size_t GetTemplateCount() override
    {
        return sizeof(mTemplates) / sizeof(GraphEditor::Template);
    }

    const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
    {
        return mTemplates[index];
    }

    const size_t GetNodeCount() override
    {
        return mNodes.size();
    }

    const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
    {
        const auto& myNode = mNodes[index];
        return GraphEditor::Node
        {
            myNode.name,
            myNode.templateIndex,
            ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
            myNode.mSelected
        };
    }

    const size_t GetLinkCount() override
    {
        return mLinks.size();
    }

    const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
    {
        return mLinks[index];
    }

    // Graph datas
    static const inline GraphEditor::Template mTemplates[] = {
        {
            IM_COL32(160, 160, 180, 255),
            IM_COL32(100, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            1,
            Array{"MyInput"},
            nullptr,
            2,
            Array{"MyOutput0", "MyOuput1"},
            nullptr
        },

        {
            IM_COL32(180, 160, 160, 255),
            IM_COL32(140, 100, 100, 255),
            IM_COL32(150, 110, 110, 255),
            3,
            nullptr,
            Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
            1,
            Array{"MyOutput0"},
            Array{ IM_COL32(200,200,200,255)}
        }
    };

    struct Node
    {
        const char* name;
        GraphEditor::TemplateIndex templateIndex;
        float x, y;
        bool mSelected;
    };

    std::vector<Node> mNodes = {
        {
            "My Node 0",
            0,
            0, 0,
            false
        },

        {
            "My Node 1",
            0,
            400, 0,
            false
        },

        {
            "My Node 2",
            1,
            400, 400,
            false
        }
    };

    std::vector<GraphEditor::Link> mLinks = { {0, 0, 1, 0} };
};

void Sandbox::onRenderImgui()
{
    static GraphEditor::Options options;
    static GraphEditorDelegate delegate;
    static GraphEditor::ViewState viewState;
    static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;
    static bool showGraphEditor = true;

    if (ImGui::CollapsingHeader("Graph Editor"))
    {
        ImGui::Checkbox("Show GraphEditor", &showGraphEditor);
        GraphEditor::EditOptions(options);
    }

    if (showGraphEditor)
    {
        ImGui::Begin("Graph Editor", NULL, 0);
        if(ImGui::Button("Fit all nodes"))
        {
            fit = GraphEditor::Fit_AllNodes;
        }
        ImGui::SameLine();
        if (ImGui::Button("Fit selected nodes"))
        {
            fit = GraphEditor::Fit_SelectedNodes;
        }
        GraphEditor::Show(delegate, options, viewState, true, &fit);

        ImGui::End();
    }
}



