#include <pch.h>
#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "EditorLayer.h"

/// <summary>
/// This is a class to call the render initialisation
/// to the screen when a layer is added to the app.
/// There is also a deconstructor to tocall the destroy method in Renderer.h.
/// This class inherits Hzn::App.
/// </summary>
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


/// <summary>
/// This is a method to call to make new layers and render them on screen.
/// This makes a new shared pointer instance app od Hzn::App.
/// And with this app calls the addLayer method to add a layer.
/// Here we have added an instance of the EditorLayer().
/// To make a new layer call "app->addLayer(new NEWLAYER());
/// </summary>
/// <returns></returns>
std::shared_ptr<Hzn::App> Hzn::createApp()
{
	auto app = std::make_shared<EditorApp>();
	app->addLayer(new EditorLayer());
	return app;
}