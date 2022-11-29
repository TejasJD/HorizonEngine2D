#include <pch.h>
#include "GameLayer.h"

GameLayer::GameLayer(const char* name) :
	Hzn::Layer(name)
{
}


GameLayer::~GameLayer()
{
	// I think it's fine for detach function to be statically resolved at compile time in this case.
	onDetach();
}

void GameLayer::onAttach()
{
	HZN_TRACE("Game Layer Attached!");
	Hzn::FrameBufferProps props;
	props.width = Hzn::App::getApp().getAppWindow().getWidth();
	props.height = Hzn::App::getApp().getAppWindow().getHeight();
	props.attachments = {
		Hzn::FrameBufferTextureFormat::RGBA8,
		Hzn::FrameBufferTextureFormat::RED_INTEGER,
		Hzn::FrameBufferTextureFormat::DEPTH24_STENCIL8
	};

	m_FrameBuffer = Hzn::FrameBuffer::create(props);
}

void GameLayer::onDetach()
{
	Hzn::ProjectManager::close();
}

void GameLayer::onUpdate(Hzn::TimeStep ts) 
{

}

void GameLayer::onEvent(Hzn::Event& e)
{

}

void GameLayer::onRenderImgui()
{
	
}