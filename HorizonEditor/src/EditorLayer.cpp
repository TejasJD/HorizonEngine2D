#include <pch.h>
#include "EditorLayer.h"


EditorLayer::EditorLayer(const char* name) :
	Hzn::Layer(name),
    m_AspectRatio(static_cast<float>(Hzn::App::getApp().getAppWindow().getWidth()) /
        static_cast<float>(Hzn::App::getApp().getAppWindow().getHeight())),
	m_CameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
{
}

void EditorLayer::onAttach()
{
	HZN_TRACE("Editor Layer Attached!");
	m_CheckerboardTexture = Hzn::Texture2D::create("assets/textures/bear.png");
	Hzn::FrameBufferProps props;
	props.width = Hzn::App::getApp().getAppWindow().getWidth();
	props.height = Hzn::App::getApp().getAppWindow().getHeight();

	m_FrameBuffer = Hzn::FrameBuffer::create(props);
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(Hzn::TimeStep ts)
{
	if(m_ViewportFocused && m_ViewportHovered) m_CameraController.onUpdate(ts);

	m_FrameBuffer->bind();
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();
	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(m_CameraController.getCamera()));

	for (int32_t i = 0; i < quads; ++i)
	{
		for (int32_t j = 0; j < quads; ++j)
		{
			// colored quads.
			auto color = glm::lerp(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), (float)(i + j) / (2 * quads));
			Hzn::Renderer2D::drawQuad({ i * 0.11f, j * 0.11f, 0.0f }, quadAngle, glm::vec3(0.1f), color);
			// textured quads.
			/*Hzn::Renderer2D::drawQuad({ i * 1.11f, j * 1.11f, 1.0f }, quadAngle, glm::vec3(1.0f), someSky);*/
		}
	}

	Hzn::Renderer2D::endScene();
	m_FrameBuffer->unbind();
}

bool EditorLayer::onWindowResize(Hzn::WindowResizeEvent& e)
{
	return false;
}

void EditorLayer::onEvent(Hzn::Event& e)
{
    /*HZN_DEBUG("Editor Layer Received Event!");*/
	if(m_ViewportFocused && m_ViewportHovered) m_CameraController.onEvent(e);

	Hzn::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hzn::WindowResizeEvent>(std::bind(&EditorLayer::onWindowResize, this, std::placeholders::_1));
}

void EditorLayer::onRenderImgui()
{
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
    ImGui::Begin("DockSpace Demo",  &p_open, window_flags);
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

            if (ImGui::MenuItem("Exit"))
            {
	            Hzn::App::getApp().close();
            }
               
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }


    // SETTINGS BEGIN.
	ImGui::Begin("Settings");
	ImGui::SliderInt("Grid Side", &quads, 5, 1000);
	ImGui::SliderFloat("Quad Angle:", &quadAngle, -180.0f, 180.0f);
	ImGui::Text("Draw calls: %d", stats.draws);
	ImGui::Text("Quads: %d", stats.quads);
	ImGui::Text("Vertices: %d", stats.vertices);
	ImGui::Text("Indices: %d", stats.indices);
	ImGui::End();
    // SETTINGS END.

	// VIEWPORT BEGIN
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");

    // get the states of the viewport.
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // ImGui layer will not block the events if the viewport is focused and hovered.
    Hzn::App::getApp().getImguiLayer()->blockEvents(!m_ViewportFocused || !m_ViewportHovered);

	glm::vec2 viewportSize = *reinterpret_cast<glm::vec2*>(&(ImGui::GetContentRegionAvail()));

    // if the viewport changes we don't get the viewport.
	if(lastViewportSize != viewportSize)
	{
		// this invalidates the current frame buffer and recreates a new frame buffer.
		m_FrameBuffer->recreate(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));

		// we recalculate camera's projection matrix as the viewport has changed, and we want to maintain the aspect ratio.
		auto& camera = m_CameraController.getCamera();
		camera.setAspectRatio(viewportSize.x / viewportSize.y);
		camera.calculateProjectionMatrix();

		// update the last viewport size to the current viewport size.
		lastViewportSize = viewportSize;
	}

	/*HZN_INFO("{0}, {1}", viewportSize.x, viewportSize.y);*/

	ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer->getColorAttachmentId()),
		{ viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
	ImGui::End();
	ImGui::PopStyleVar();
    // VIEWPORT END.

    ImGui::End();
    // DOCKING END.
}