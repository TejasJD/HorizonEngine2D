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

	bool onKeyPressedEvent(Hzn::KeyPressedEvent& event)
	{
		return false;
	}

private:
	std::shared_ptr<Hzn::VertexArray> m_VertexArray;
	std::shared_ptr<Hzn::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hzn::ElementBuffer> m_ElementBuffer;
	std::shared_ptr<Hzn::Shader> m_Shader;
	std::shared_ptr<Hzn::Camera> camera;
	std::vector<glm::vec3> cubePositions;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	float previousSecond = 0.0f; // Keeps track of previous second
	float frameCount = 0.0f;
	float fps = 0;
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
