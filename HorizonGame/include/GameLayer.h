#pragma once

#include <HorizonEngine.h>

class GameLayer : public Hzn::Layer
{
public:

	GameLayer(const char* name = "Game Layer");
	virtual ~GameLayer();
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Hzn::TimeStep deltaTime) override;
	virtual void onEvent(Hzn::Event& event) override;
	virtual void onRenderImgui() override;

private:
	std::shared_ptr<Hzn::Scene> m_ActiveScene;

	int32_t m_PlayerObjectId = std::numeric_limits<uint32_t>::max();

	std::vector<float> attackTimers{ 1.0f, 1.0f, 1.0f, 1.0f };

	std::vector<std::string> sceneNames{ "mainMenu.scene", "level1.scene", "level2.scene", "level3.scene" };

	void playerCollisionCallback(Hzn::GameObject obj);
};