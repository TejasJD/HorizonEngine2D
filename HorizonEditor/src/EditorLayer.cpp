#include <pch.h>
#include "EditorLayer.h"

EditorLayer::EditorLayer(const char* name) :
	Hzn::Layer(name),
    m_AspectRatio(static_cast<float>(Hzn::App::getApp().getAppWindow().getWidth()) /
        static_cast<float>(Hzn::App::getApp().getAppWindow().getHeight())),
	m_CameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
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

    for(const auto& x : list)
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
	if(m_ViewportFocused && m_ViewportHovered) m_CameraController.onUpdate(ts);

	m_FrameBuffer->bind();

    // here, in case if the framebuffer is re-created, and the last known
    // viewport size does not match the viewport size of the new framebuffer, then
    // we update all the camera components to the proper aspect ratio, and update the last known viewport size.
    auto& props = m_FrameBuffer->getProps();
    lastViewportSize = m_Scene->onViewportResize(props.width, props.height);
    m_CameraController.getCamera().setAspectRatio(lastViewportSize.x / lastViewportSize.y);

	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	/*Hzn::Renderer2D::beginScene(m_CameraController.getCamera());*/
    // update the scene.
    m_Scene->onUpdate(ts);
    // unbind the current framebuffer.
    /*Hzn::Renderer2D::endScene();*/

	m_FrameBuffer->unbind();
}

void EditorLayer::onEvent(Hzn::Event& e)
{
	if(m_ViewportFocused && m_ViewportHovered) m_CameraController.onEvent(e);
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

    // OBJECT HIERARCHY BEGIN
    drawHierarchy();
    // OBJECT HIERARCHY END

    /*static bool show = true;*/
    // SETTINGS BEGIN.
	ImGui::Begin("Components");
    if (selectedObject != "") {
        auto selectedObj = m_Scene->getGameObject(selectedObject);
        Hzn::displayIfExists<Hzn::NameComponent>(selectedObj);
        Hzn::displayIfExists<Hzn::TransformComponent>(selectedObj);
        Hzn::displayIfExists<Hzn::RenderComponent>(selectedObj);
        Hzn::displayIfExists<Hzn::CameraComponent>(selectedObj);
    }
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

    // if viewport size changes then we re-create the frame buffer.
	glm::vec2 viewportSize = *reinterpret_cast<glm::vec2*>(&(ImGui::GetContentRegionAvail()));
	if(lastViewportSize != viewportSize)
	{
		m_FrameBuffer->recreate(viewportSize.x, viewportSize.y);
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

void EditorLayer::drawHierarchy()
{
    ImGui::Begin("Object Hierarchy");
    auto list = m_Scene->getAllRootObjects();

    openHierarchyPopup = false;

    for (const auto& x : list)
    {
        drawObjects(m_Scene->getGameObject(x));
    }

    if (openHierarchyPopup) {
        if (ImGui::BeginPopup("HierarchyObjectPopup")) {
            HZN_CORE_DEBUG("Test");

            if (ImGui::MenuItem("Copy", NULL, false)) {
                // Do stuff here
            }
            if (ImGui::MenuItem("Paste", NULL, false)) {
                // Do stuff here 
            }
            if (ImGui::MenuItem("Duplicate", NULL, false)) {
                // Do stuff here 
            }
            if (ImGui::MenuItem("Delete", NULL, false)) {
                // Do stuff here
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Create Empty", NULL, false)) {
                // Do stuff here
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void EditorLayer::drawObjects(const Hzn::GameObject& object)
{
    std::vector<Hzn::GameObject> list = object.getChildren();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
    
    if (list.size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    if (selectedObject == object.getComponent<Hzn::NameComponent>().m_Name) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if(ImGui::TreeNodeEx(object.getComponent<Hzn::NameComponent>().m_Name.c_str(), flags))
    {
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            selectedObject = object.getComponent<Hzn::NameComponent>().m_Name.c_str();
        }


        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            selectedObject = object.getComponent<Hzn::NameComponent>().m_Name.c_str();
            
            ImGui::OpenPopup("HierarchyObjectPopup");
        }

        openHierarchyPopup |= true;

        for (const auto& x : list)
        {
            drawObjects(x);
        }

        if (list.size() > 0)
            ImGui::TreePop();
    }
}
