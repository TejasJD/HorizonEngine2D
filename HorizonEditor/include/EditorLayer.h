#pragma once

#include "HorizonEngine.h"

class EditorLayer : public Hzn::Layer
{
public:
	EditorLayer(const char* name = "Editor Layer");
	virtual ~EditorLayer() {}
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Hzn::TimeStep deltaTime) override;
	virtual void onEvent(Hzn::Event& event) override;
	virtual void onRenderImgui() override;
	bool onWindowResize(Hzn::WindowResizeEvent& e);

private:
	int32_t quads = 10;
	float quadAngle = 0.0f;
	float m_AspectRatio = 1.0f;
	glm::vec2 lastViewportSize = { 0.0f, 0.0f };
	Hzn::OrthographicCameraController m_CameraController;
	std::shared_ptr<Hzn::Texture2D> m_CheckerboardTexture;
	std::shared_ptr<Hzn::FrameBuffer> m_FrameBuffer;
};