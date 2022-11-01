#pragma once

#ifndef HZNAPP_SANDBOX_H
#define HZNAPP_SANDBOX_H

enum class Direction
{
	Up = 0, Down, Left, Right
};

class Sandbox : public Hzn::Layer
{
public:
	Sandbox();
	virtual ~Sandbox() {}

	void onAttach() override;


	void onDetach() override;


	void onUpdate(Hzn::TimeStep deltaTime) override;


	void onEvent(Hzn::Event& event) override;


	void onRenderImgui() override;

private:
	float quadAngle = 0.0f;
	float m_AspectRatio = 0.0f;
	float m_PreviousFrame = 0.0f;
	uint32_t m_FrameCounter = 0;
	float m_Fps = 0.0f;
	int32_t quads = 10;

	Hzn::OrthographicCameraController m_CameraController;
	std::shared_ptr<Hzn::Texture2D> someSky;
	std::shared_ptr<Hzn::Texture2D> checkerBoard;
};

#endif // !HZNAPP_SANDBOX_H
