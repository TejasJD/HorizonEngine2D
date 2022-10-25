#pragma once
#include "HorizonEngine.h"

#ifndef HZNAPP_HZNAPP_H
#define HZNAPP_HZNAPP_H

// ********** Sample Layer **********

class SampleLayer : public Hzn::Layer
{
public:
	SampleLayer(const std::string& name = "Sample Layer");

	// Inherited via Layer
	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;

};

// ********** Edior Layer **********

class EditorLayer : public Hzn::Layer
{
private:
	Hzn::Scene* openScene;
	std::string projectRootFolder;
	std::string projectPath;
	std::vector<std::shared_ptr<Hzn::TreeNode<std::string>>> nodes;
	std::string contextObject = "";
	bool openContext = false;
public:
	EditorLayer(const std::string& name = "Editor Layer");

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
	std::pair<bool, uint32_t> drawProjectExplorerNode(const std::filesystem::path& path);

	void drawConsole();
	bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);

	void drawHierarchyNode(std::shared_ptr<Hzn::TreeNode<std::string>> node);
};

class HznApp : public Hzn::App
{
public:
	HznApp()
	{
		/*addLayer(new SampleLayer());*/
		addLayer(new EditorLayer());

	
	}
	~HznApp() {}
};

#endif // !HZNAPP_HZNAPP_H
