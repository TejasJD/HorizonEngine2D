#pragma once

#ifndef HZNAPP_HZNAPP_H
#define HZNAPP_HZNAPP_H
// ********** Edior Layer **********

class EditorLayer : public Hzn::Layer
{
private:
	Hzn::Scene* openScene;
	std::string projectRootFolder;
	std::string assetPath;
	std::string projectPath ="Project (No open project)";;
	std::vector<std::shared_ptr<Hzn::TreeNode<std::string>>> nodes;
	std::string contextObject = "";
	bool openContext = false;
	bool dirOpenContext = false;
	bool clickStatus = false;
	std::shared_ptr<Hzn::GameObject> copiedGameObject;
	std::map<std::string, Hzn::AudioSource*> audioFileMap;
	std::filesystem::path m_CurrentDirectory;
	std::shared_ptr<Hzn::Texture> folderIcon;
	std::map<std::string, std::shared_ptr<Hzn::Texture>> fileIconMap;
	std::shared_ptr<Hzn::Texture> fileIcon;
public:
	EditorLayer(const char* name = "Editor Layer");

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;

private:
	void setupDockSpace(bool* pOpen);
	void dockWidgets(ImGuiID dockspace_id);
	void drawMenuBar(bool* pOpen);
	void drawScene();
	void drawObjectBehaviour();
	void drawHierarchy();
	void drawProjectExplorer(std::string directoryPath);
	void drawProjectExplorerNode(const std::filesystem::path& path);

	void drawConsole();
	bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);

	void drawHierarchyNode(std::shared_ptr<Hzn::TreeNode<std::string>> node);

	void drawAudio(std::string directoryPath);
	void drawAudioNode(const std::filesystem::path& path);

	void drawContentBrowser();
};

class HznApp : public Hzn::App
{
public:
	HznApp()
	{
		/*Hzn::App::getApp().getAppWindow().setVsync(false);*/
		Hzn::Renderer2D::init();
	}
	~HznApp()
	{
		Hzn::Renderer2D::destroy();
	}
};

#endif // !HZNAPP_HZNAPP_H
