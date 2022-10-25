#pragma once

#ifndef HZNAPP_HZNAPP_H
#define HZNAPP_HZNAPP_H

// ********** Sample Layer **********

class SampleLayer : public Hzn::Layer
{
public:
	SampleLayer(const std::string& name = "Sample Layer");

	// Inherited via Layer
	virtual void onUpdate(Hzn::TimeStep timestep) override;

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;

	void mouseMovementCamera(Hzn::TimeStep deltaTime);

	bool onMouseScroll(Hzn::MouseScrolledEvent& event);

private:
	std::shared_ptr<Hzn::VertexArray> vertexArray;
	std::shared_ptr<Hzn::VertexBuffer> vertexBuffer;
	std::shared_ptr<Hzn::ElementBuffer> elementBuffer;
	std::shared_ptr<Hzn::Shader> shader, textureShader;
	std::shared_ptr<Hzn::Camera> camera;

	std::shared_ptr<Hzn::Texture> texture;
	std::shared_ptr<Hzn::Texture> logoTexture;

	glm::vec3 gridColor = { 0.2f, 0.0f, 0.5f };

	float previousSecond = 0.0f;
	float fps = 0.0f;
	float frameCount = 0.0f;
	float rotation = 90.0f;
};

// ********** Edior Layer **********

class EditorLayer : public Hzn::Layer
{
private:
	Hzn::Scene* openScene;
	std::string projectRootFolder;
	std::vector<std::shared_ptr<Hzn::TreeNode<std::string>>> nodes;
	std::string contextObject = "";
	bool openContext = false;
	std::shared_ptr<Hzn::GameObject> copiedGameObject;
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
	void drawProjectExplorer();
	void drawConsole();
	bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);

	void drawHierarchyNode(std::shared_ptr<Hzn::TreeNode<std::string>> node);
};

class HznApp : public Hzn::App
{
public:
	HznApp() {}
	~HznApp() {}
};

#endif // !HZNAPP_HZNAPP_H
