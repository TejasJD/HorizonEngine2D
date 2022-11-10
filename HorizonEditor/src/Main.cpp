#include <pch.h>
#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "EditorLayer.h"

class EditorApp : public Hzn::App
{
public:
	EditorApp()
	{
		Hzn::Renderer2D::init();
	}

	virtual ~EditorApp()
	{
		Hzn::Renderer2D::destroy();
	}
};

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<EditorApp>();
	app->addLayer(new EditorLayer());
	return app;
}