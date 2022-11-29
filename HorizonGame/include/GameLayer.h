#pragma once

#include <HorizonEngine.h>

class EditorLayer : public Hzn::Layer
{
public:

	EditorLayer(const char* name = "Game Layer");
	virtual ~EditorLayer();
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Hzn::TimeStep deltaTime) override;
	virtual void onEvent(Hzn::Event& event) override;
	virtual void onRenderImgui() override;

private:
	bool onKeyPressed(Hzn::KeyPressedEvent& e);
	bool onMouseButtonPressed(Hzn::MouseButtonPressedEvent& e);

	std::shared_ptr<Hzn::FrameBuffer> m_FrameBuffer;

	std::vector<std::string> sceneNames{ "mainMenu.scene", "level1.scene", "level2.scene", "level3.scene" };
};