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
	/*Hzn::FrameBufferProps props;
	props.width = Hzn::App::getApp().getAppWindow().getWidth();
	props.height = Hzn::App::getApp().getAppWindow().getHeight();
	props.attachments = {
		Hzn::FrameBufferTextureFormat::RGBA8,
		Hzn::FrameBufferTextureFormat::RED_INTEGER,
		Hzn::FrameBufferTextureFormat::DEPTH24_STENCIL8
	};

	m_FrameBuffer = Hzn::FrameBuffer::create(props);*/

	// TODO: load the first scene
	m_ActiveScene = Hzn::SceneManager::open("assets/scenes/Level2.scene");

	m_PlayerObjectId = m_ActiveScene->getGameObjectByName("Player").getObjectId();

	m_ActiveScene->onStart();
}

void GameLayer::onDetach()
{
	Hzn::SceneManager::close();
}

void GameLayer::onUpdate(Hzn::TimeStep ts) 
{
	auto width = Hzn::App::getApp().getAppWindow().getWidth();
	auto height = Hzn::App::getApp().getAppWindow().getHeight();
	m_ActiveScene->onViewportResize(width, height);

	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	m_ActiveScene->onUpdate(ts);
	//Hzn::Renderer2D::beginScene()
}

void GameLayer::onEvent(Hzn::Event& e)
{
	if (Hzn::Input::keyPressed(Hzn::Key::W) || Hzn::Input::keyPressed(Hzn::Key::Up)) {
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addForce(glm::vec2(0.0f, 1500.0f));
	}

	if (Hzn::Input::keyPressed(Hzn::Key::A) || Hzn::Input::keyPressed(Hzn::Key::Left)) {
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addForce(glm::vec2(-200.0f, 0.0f));
	}

	if (Hzn::Input::keyPressed(Hzn::Key::D) || Hzn::Input::keyPressed(Hzn::Key::Right)) {
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addForce(glm::vec2(200.0f, 0.0f));
	}
}

void GameLayer::onRenderImgui()
{
	
}