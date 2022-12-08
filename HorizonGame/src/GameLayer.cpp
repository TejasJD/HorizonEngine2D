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

	m_PlatformIds.push_back(m_ActiveScene->getGameObjectByName("Ground").getObjectId());
	m_PlatformIds.push_back(m_ActiveScene->getGameObjectByName("Platform 1").getObjectId());
	m_PlatformIds.push_back(m_ActiveScene->getGameObjectByName("Platform 2").getObjectId());

	for (int i = 0; i < 4; i++) {
		Hzn::GameObject enemy = m_ActiveScene->getGameObjectByName("Enemy " + std::to_string(i + 1));
		m_EnemyIds.push_back(enemy.getObjectId());
		enemy.getComponent<Hzn::BoxCollider2DComponent>().m_IsSensor = true;
		enemy.getComponent<Hzn::RigidBody2DComponent>().m_Type = Hzn::RigidBody2DComponent::BodyType::Kinematic;
	}

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

	Hzn::GameObject player = m_ActiveScene->getGameObjectById(m_PlayerObjectId);
	auto& playerTransform = player.getComponent<Hzn::TransformComponent>();

	// Check collision for enemies
	for (int i = 0; i < m_EnemyIds.size(); i++) {
		Hzn::GameObject enemy = m_ActiveScene->getGameObjectById(m_EnemyIds[i]);
		auto& enemyTransform = enemy.getComponent<Hzn::TransformComponent>();
		if (enemyTransform.m_Translation.x - 0.5f < playerTransform.m_Translation.x &&
			enemyTransform.m_Translation.x + 0.5f > playerTransform.m_Translation.x &&
			enemyTransform.m_Translation.y - 0.5f < playerTransform.m_Translation.y &&
			enemyTransform.m_Translation.y + 0.5f > playerTransform.m_Translation.y) {
			playerCollisionCallback(enemy);
		}
	}

	// Check collision for projectiles
	std::vector<Hzn::GameObject> projectiles = m_ActiveScene->getGameObjectsByName("Projectile");
	for (std::vector<Hzn::GameObject>::reverse_iterator i = projectiles.rbegin(); i != projectiles.rend(); ++i) {
		auto& projectileTransform = (*i).getComponent<Hzn::TransformComponent>();
		if (projectileTransform.m_Translation.x - 0.5f < playerTransform.m_Translation.x &&
			projectileTransform.m_Translation.x + 0.5f > playerTransform.m_Translation.x &&
			projectileTransform.m_Translation.y - 0.5f < playerTransform.m_Translation.y &&
			projectileTransform.m_Translation.y + 0.5f > playerTransform.m_Translation.y) {
			playerCollisionCallback(*i);
		}
	}

	// Check collision with ground

	m_ActiveScene->onUpdate(ts);
}

void GameLayer::onEvent(Hzn::Event& e)
{
	if (Hzn::Input::keyPressed(Hzn::Key::W) || Hzn::Input::keyPressed(Hzn::Key::Up)) {
		if (m_AllowJump) {
			m_ActiveScene->getGameObjectById(m_PlayerObjectId).getComponent<Hzn::RigidBody2DComponent>().addImpulseForce(glm::vec2(0.0f, 8.0f));
			//m_AllowJump = false;
		}
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
	std::string objName = obj.getComponent<Hzn::NameComponent>().m_Name;
	if (objName.find("Enemy") != std::string::npos) {
		// Destroy enemy
		//obj.destroy();
		HZN_CORE_DEBUG("Enemy hit!");
	}
	else if (objName == "Projectile") {
		// Maybe reduce player's health instead of killing them instantly
		//m_ActiveScene = Hzn::SceneManager::open("assets/scenes/Level2.scene");
		HZN_CORE_CRITICAL("Player hit!");
	}
}