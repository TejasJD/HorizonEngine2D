#include "Physics2DManager.h"

#include <HorizonEngine/Components/PhysicsComponent.h>

namespace Hzn {
	b2Vec2 Physics2DManager::m_Gravity(0, -9.81);

	bool Physics2DManager::m_DoSleep = true;
	int32 Physics2DManager::m_VelocityIterations = 5;
	int32 Physics2DManager::m_PositionIterations = 3;

	b2World* Physics2DManager::m_World = nullptr;

	Physics2DContactListener Physics2DManager::contactListener;

	void Physics2DManager::generateBody(GameObject& obj)
	{
		setShape(obj);

		if (obj.hasComponent<BoxCollider2DComponent>()) {
			auto& boxCollider2DComponent = obj.getComponent<BoxCollider2DComponent>();

			if (!boxCollider2DComponent.m_Body) {
				auto& transformComponent = obj.getComponent<TransformComponent>();

				b2BodyDef bodyDef;
				b2FixtureDef fixtureDef;

				// Calculate body initial position and rotation
				GameObject parent = obj.getParent();
				glm::vec3 startPosition = transformComponent.m_Translation + glm::vec3(boxCollider2DComponent.m_Offset.x, boxCollider2DComponent.m_Offset.y, transformComponent.m_Translation.z);
				float rotation = transformComponent.m_Rotation.z;
				while (parent) {
					auto& t = parent.getComponent<TransformComponent>();
					startPosition += t.m_Translation;
					rotation += t.m_Rotation.z;

					parent = parent.getParent();
				}

				bodyDef.position.Set(startPosition.x, startPosition.y);
				bodyDef.angle = rotation;

				// Set body options if a rigidbody is present on the object
				if (obj.hasComponent<Rigidbody2DComponent>()) {
					auto& rigidbody = obj.getComponent<Rigidbody2DComponent>();

					switch (rigidbody.m_BodyType) {
					case 0: // Dynamic body
						bodyDef.type = b2_dynamicBody;
						break;
					case 1: // Kinematic body
						bodyDef.type = b2_kinematicBody;
						break;
					case 2: // Static body
						bodyDef.type = b2_staticBody;
					}

					bodyDef.gravityScale = rigidbody.m_GravityScale;
					bodyDef.linearDamping = rigidbody.m_LinearDamping;
					bodyDef.angularDamping = rigidbody.m_AngularDamping;
				}

				//int id = obj.getObjectId();
				bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&obj);

				// Create the body and add it to the world
				boxCollider2DComponent.m_Body = Physics2DManager::addBody(bodyDef);

				fixtureDef.isSensor = boxCollider2DComponent.m_IsTrigger;
				fixtureDef.shape = &boxCollider2DComponent.m_Shape;

				boxCollider2DComponent.m_Fixture = boxCollider2DComponent.m_Body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Physics2DManager::setShape(GameObject& obj) {
		if (obj.hasComponent<BoxCollider2DComponent>()) {
			auto& boxCollider = obj.getComponent<BoxCollider2DComponent>();
			boxCollider.m_Shape.SetAsBox(boxCollider.m_Size.x, boxCollider.m_Size.y);
		}
	}
}