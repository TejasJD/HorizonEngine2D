#include <pch.h>
#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "GameLayer.h"

class GameApp : public Hzn::App
{
public:
	GameApp()
	{
		Hzn::Renderer2D::init();
	}

	virtual ~GameApp()
	{
		Hzn::Renderer2D::destroy();
	}
};

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<GameApp>();
	app->addLayer(new GameLayer());
	return app;
}