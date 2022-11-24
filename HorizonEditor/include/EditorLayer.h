#pragma once

#include "HorizonEngine.h"


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
	virtual void openScene(const std::filesystem::path& filepath);

private:


	void drawHierarchy();
	void drawObjects(Hzn::GameObject& object);
	//void openProject();
	int32_t quads = 10;
	float quadAngle = 0.0f;
	float m_AspectRatio = 1.0f;
	glm::vec2 lastViewportSize = { 0, 0 };
	Hzn::OrthographicCameraController m_EditorCameraController;
	std::shared_ptr<Hzn::Texture2D> m_CheckerboardTexture;
	std::shared_ptr<Hzn::FrameBuffer> m_FrameBuffer;
	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	bool m_PlayMode = false;

	std::shared_ptr<Hzn::Scene> m_Scene;
	Hzn::GameObject m_SquareObject;
	Hzn::GameObject m_SquareObject2;
	Hzn::GameObject m_Camera;
	float m_CameraZoom = 1.0f;

	//Hzn::AssetManager assetManager;
	//std::map<std::string, std::shared_ptr<Hzn::Sprite2D>> spriteMap;
	//std::filesystem::path m_CurrentDirectory;
	//std::shared_ptr<Hzn::Texture> folderIcon;
	//std::shared_ptr<Hzn::Texture> fileIcon;
	//std::string projectRootFolder;
	std::string assetPath;
	std::string currentScenePath;

	std::vector<std::string> rootObjects;

	std::string selectedObject = "";
	uint32_t selectedObjectId = std::numeric_limits<uint32_t>::max();
	Hzn::GameObject copiedGameObject;
	bool openHierarchyPopup = false;

	glm::vec2 m_ViewportBounds[2]{{0.0f, 0.0f}, {0.0f, 0.0f}};

	std::shared_ptr<Hzn::Project> m_ActiveProject;
	//bool request_NewScene = false;
	bool request_OpenScene = false;
	//bool canCreateProject = false;
};