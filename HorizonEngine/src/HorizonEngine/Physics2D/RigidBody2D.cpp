#include "pch.h"
#include "RigidBody2D.h"

namespace Hzn {
	namespace Physics2D {
		Rigidbody2D::Rigidbody2D() {

		}

		Rigidbody2D::~Rigidbody2D() {
			PhysicsWorld2D::world->DestroyBody(body);
		}


		void Rigidbody2D::setPosition(glm::vec2 position) {
			body->SetTransform(b2Vec2(position.x, position.y), transform.lock().get()->rotation);
		}

		void Rigidbody2D::setRotation(float rotation) {
			glm::vec2 position = transform.lock().get()->position;
			body->SetTransform(b2Vec2(position.x, position.y), rotation);
		}

		void Rigidbody2D::init() {
			b2BodyDef bodyDef;
			switch (type)
			{
			case BodyType::Static:
				bodyDef.type = b2_staticBody;
				break;
			case BodyType::Dynamic:
				bodyDef.type = b2_dynamicBody;
				break;
			case BodyType::Kinematic:
				bodyDef.type = b2_kinematicBody;
			default:
				bodyDef.type = b2_staticBody;
				break;
			}
			bodyDef.position.Set(transform.lock()->position.x, transform.lock()->position.y);
			bodyDef.angle = transform.lock()->rotation;

			// Create the body
			body = PhysicsWorld2D::world->CreateBody(&bodyDef);

			// Create and add it's colliders
			std::vector<Component*>* colliders = transform.lock()->gameObject->getComponentsInChildren(ComponentType::C_Collider2D);
			for (int i = 0; i < colliders->size(); i++) {
				// Get the current collider
				Collider2D* collider = static_cast<Collider2D*>(colliders->at(i));
				std::vector<b2PolygonShape>* shapes = collider->generateCollider();
				// Loop through all the shapes for the current collider
				// This is done because capsule collider is composed of 3 shapes
				for (int j = 0; j < shapes->size(); j++) {
					// For all transforms that are parents of the current collider's transform, sum the local 
					// offset from the center of the gameObject to which the rigidbody2D is attached to.
					glm::vec2 offset(0, 0);
					std::weak_ptr<Transform> t = collider->transform;
					do {
						offset += t.lock()->localPosition;
						t = t.lock()->parent;
					} while (t.lock().get() != transform.lock().get());

					// Set the correct offset
					shapes->at(j).m_centroid = b2Vec2(offset.x, offset.y);

					b2FixtureDef boxFixtureDef;
					boxFixtureDef.shape = &(shapes->at(j));
					boxFixtureDef.density = 1;
					body->CreateFixture(&boxFixtureDef);
				}
			}

			// Sets the body's initial position and rotation
			body->SetTransform(b2Vec2(transform.lock()->position.x, transform.lock()->position.y), transform.lock()->rotation * Math::deg2rad);
		}

		void Rigidbody2D::update() {
			b2Transform t = body->GetTransform();
			transform.lock().get()->position = b2Vec2(t.p.x, t.p.y);
			transform.lock().get()->rotation = t.q.GetAngle();
		}
	}
}