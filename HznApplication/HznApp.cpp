#include "pch.h"
#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "Sandbox.h"
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<HznApp>();
	//app->addLayer(new EditorLayer());
	app->addLayer(new Sandbox());
	return app;
}