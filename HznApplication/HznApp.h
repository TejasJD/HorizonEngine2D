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
	std::shared_ptr<Hzn::VertexArray> m_VertexArray;
	std::shared_ptr<Hzn::VertexArray> lightVertexArray;
	std::shared_ptr<Hzn::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hzn::ElementBuffer> m_ElementBuffer;
	std::shared_ptr<Hzn::Shader> lightSourceShader;
	std::shared_ptr<Hzn::Shader> lightShader;
	std::shared_ptr<Hzn::Camera> camera;
	std::vector<glm::vec3> cubePositions;

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 lightSource = { 1.0f, 1.0f, 1.0f }; // color of the light source.

	float lastFrame = 0.0f; // Time of last frame
	float previousSecond = 0.0f; // indicates the start of previous second.
	float frameCount = 0.0f; // Keeps track of the number of frames (reset every second).
	float fps = 0.0f; // number that indicates frames per second.
	float pitch = 0.0f; // angle for rotation along x-axis.
	float yaw = 0.0f; // angle for rotation along y-axis.
	float fov = 0.0f; // field of view (for the case of perspective projection).


	float lastX = Hzn::App::getApp().getAppWindow().getWidth() / 2; // keeps track of last X position of mouse.
	float lastY = Hzn::App::getApp().getAppWindow().getHeight() / 2; // keeps track of last Y position of mouse.

	bool firstFrame = true; // simple bool for utility purposes, true only for the first frame.
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
