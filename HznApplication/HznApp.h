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
public:
	EditorLayer(const std::string& name = "Editor Layer");

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;
};


class HznApp : public Hzn::App
{
public:
	HznApp() {}
	~HznApp() {}
};

#endif // !HZNAPP_HZNAPP_H
