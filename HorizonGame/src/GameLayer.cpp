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

	// Add collision callbacks
	//m_ActiveScene->getGameObjectById(m_PlayerObjectId).addCollisionEnetrCallback();

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

	for (int i = 0; i < attackTimers.size(); i++)
	if (attackTimers.at(i) <= 0.0f) {
		attackTimers.at(i) = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));;

		Hzn::GameObject p = m_ActiveScene->createGameObject("Projectile");
		p.getComponent<Hzn::TransformComponent>().m_Translation = m_ActiveScene->getGameObjectByName("Enemy " + std::to_string(i + 1)).getComponent<Hzn::TransformComponent>().m_Translation;
		p.getComponent<Hzn::TransformComponent>().m_Scale = glm::vec3(0.5f, 0.5f, 1.0f);
		p.addComponent<Hzn::BoxCollider2DComponent>();
		p.addComponent<Hzn::RigidBody2DComponent>();
		p.addComponent<Hzn::RenderComponent>();

		p.getComponent<Hzn::RigidBody2DComponent>().m_Type = Hzn::RigidBody2DComponent::BodyType::Dynamic;
		m_ActiveScene->addBody(p);
		p.getComponent<Hzn::RigidBody2DComponent>().addImpulseForce(glm::vec2(-1.0f, 1.0f));
	}
	else {
		attackTimers.at(i) -= ts;
	}

	m_ActiveScene->onUpdate(ts);
}

void GameLayer::onEvent(Hzn::Event& e)
{
	if (Hzn::Input::keyPressed(Hzn::Key::W) || Hzn::Input::keyPressed(Hzn::Key::Up)) {
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addImpulseForce(glm::vec2(0.0f, 8.0f));
	}

	if (Hzn::Input::keyPressed(Hzn::Key::A) || Hzn::Input::keyPressed(Hzn::Key::Left)) {
		// Set rigidbody velocity instead of adding force
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addImpulseForce(glm::vec2(-2.0f, 0.0f));
	}

	if (Hzn::Input::keyPressed(Hzn::Key::D) || Hzn::Input::keyPressed(Hzn::Key::Right)) {
		// Set rigidbody velocity instead of adding force
		m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addImpulseForce(glm::vec2(2.0f, 0.0f));
	}
}

void GameLayer::onRenderImgui()
{
	
}

void GameLayer::playerCollisionCallback(Hzn::GameObject obj) {
	if (obj.getComponent<Hzn::NameComponent>().m_Name.find("Enemy") != std::string::npos) {
		// Destroy enemy
	}
	else if (obj.getComponent<Hzn::NameComponent>().m_Name == "Projectile") {
		// Maybe reduce player's health instead of killing them instantly
		m_ActiveScene = Hzn::SceneManager::open("assets/scenes/Level2.scene");
	}
}