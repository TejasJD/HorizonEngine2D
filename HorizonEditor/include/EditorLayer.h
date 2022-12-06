#pragma once

#include <HorizonEngine.h>


struct EditorData {
	static std::shared_ptr<Hzn::Scene> s_Scene_Active;
	static std::shared_ptr<Hzn::Project> m_Project_Active;
};


class EditorLayer : public Hzn::Layer
{
public:

	EditorLayer(const char* name = "Editor Layer");
	virtual ~EditorLayer();
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Hzn::TimeStep deltaTime) override;
	virtual void onEvent(Hzn::Event& event) override;
	virtual void onRenderImgui() override;
	void openScene(const std::filesystem::path& filepath);

private:
	bool onKeyPressed(Hzn::KeyPressedEvent& e);
	bool onMouseButtonPressed(Hzn::MouseButtonPressedEvent& e);

	void drawHierarchy();
	void drawObjects(Hzn::GameObject& object);

	void copyObject();
	void pasteObject();
	void duplicateObject();
	void createObject();
	void deleteObject();

	float m_AspectRatio = 1.0f;
	glm::vec2 lastViewportSize = { 0, 0 };
	Hzn::OrthographicCameraController m_EditorCameraController;
	std::shared_ptr<Hzn::Texture2D> m_CheckerboardTexture;
	std::shared_ptr<Hzn::FrameBuffer> m_FrameBuffer;
	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	std::shared_ptr<Hzn::Scene> m_Scene;
	Hzn::GameObject m_SquareObject;
	Hzn::GameObject m_SquareObject2;
	Hzn::GameObject m_Camera;
	float m_Ts = 0.0f;

	std::vector<std::string> rootObjects;

	uint32_t m_SelectedObjectId = std::numeric_limits<uint32_t>::max();
	int32_t m_HoveredObjectId = -1;

	uint32_t m_CopiedObjectId = std::numeric_limits<uint32_t>::max();
	bool openHierarchyPopup = false;

	glm::vec2 m_ViewportBounds[2]{ {0.0f, 0.0f}, {0.0f, 0.0f} };

	std::shared_ptr<Hzn::Project> m_ActiveProject;
	ImGuizmo::OPERATION m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

	// key press combination check
	bool m_CtrlPressed = false;
};