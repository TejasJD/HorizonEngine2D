#include "pch.h"
#include "RigidBody2D.h"

namespace Hzn {
	Rigidbody2D::Rigidbody2D() {
		values = new std::map<std::string, std::any>();

		setField("type", 0);
		setField("size", glm::vec2(1, 1));
		setField("offset", glm::vec2(0, 0));
		setField("transform", NULL);
	}

	Rigidbody2D::~Rigidbody2D() {
		PhysicsWorld2D::world->DestroyBody(body);
	}

	void Rigidbody2D::setField(std::string k, std::any v) {
		values->insert_or_assign(k, v);
	}

	std::any Rigidbody2D::getField(std::string k) {
		return values->find(k)->second;
	}

	std::vector<std::string>* Rigidbody2D::stringify() {
		std::vector<std::string>* content = new std::vector<std::string>();

		content->push_back("component:" + getComponentType() + "\n");
		content->push_back("values:[\n");
		float type = std::any_cast<float>(getField("type"));
		content->push_back("\ttype:float:" + std::to_string(type) + "\n");
		glm::vec2 size = std::any_cast<glm::vec2>(getField("size"));
		content->push_back("\tsize:vec2:" + std::to_string(size.x) + "," + std::to_string(size.y) + "\n");
		glm::vec2 offset = std::any_cast<glm::vec2>(getField("offset"));
		content->push_back("\toffset:vec2:" + std::to_string(offset.x) + "," + std::to_string(offset.y) + "\n");
		std::shared_ptr<Component> transform;
		try {
			transform = std::any_cast<std::shared_ptr<Component>>(getField("transform"));
		}
		catch (const std::bad_any_cast& e) {
			transform = NULL;
		}
		if (transform == NULL) {
			content->push_back("\ttransform:Transform:NULL\n");
		}
		else {
			std::shared_ptr<GameObject> go = std::any_cast<std::shared_ptr<GameObject>>(transform->getField("gameObject"));
			content->push_back("\ttransform:Transform:" + go->name + "\n");
		}
		content->push_back("]\n");

		return content;
	}

	std::map<std::string, std::any>* Rigidbody2D::getValues() {
		return values;
	}

	void Rigidbody2D::setPosition(glm::vec2 position) {
		std::shared_ptr<Transform> transform = std::any_cast<std::shared_ptr<Transform>>(getField("transform"));
		body->SetTransform(b2Vec2(position.x, position.y), std::any_cast<float>(transform->getField("rotation")));
	}

	void Rigidbody2D::setRotation(float rotation) {
		std::shared_ptr<Transform> transform = std::any_cast<std::shared_ptr<Transform>>(getField("transform"));
		glm::vec2 position = std::any_cast<glm::vec2>(transform->getField("position"));
		body->SetTransform(b2Vec2(position.x, position.y), rotation);
	}

	void Rigidbody2D::init() {
		b2BodyDef bodyDef;
		BodyType type = static_cast<BodyType>(std::any_cast<int>(getField("type")));
		std::shared_ptr<Transform> transform = std::any_cast<std::shared_ptr<Transform>>(getField("transform"));
		glm::vec2 position = std::any_cast<glm::vec2>(transform->getField("position"));
		float rotation = std::any_cast<float>(transform->getField("rotation"));
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
		bodyDef.position.Set(position.x, position.y);
		bodyDef.angle = rotation;

		// Create the body
		body = PhysicsWorld2D::world->CreateBody(&bodyDef);

		// Create and add it's colliders
		std::vector<std::shared_ptr<Component>>* colliders = std::any_cast<std::shared_ptr<GameObject>>(transform->getField("gameObject"))->getComponentsInChildren("Collider2D");
		for (int i = 0; i < colliders->size(); i++) {
			// Get the current collider
			std::shared_ptr<BoxCollider2D> collider = std::dynamic_pointer_cast<BoxCollider2D>(colliders->at(i));
			std::vector<b2PolygonShape>* shapes = collider->generateCollider();
			// Loop through all the shapes for the current collider
			// This is done because capsule collider is composed of 3 shapes
			for (int j = 0; j < shapes->size(); j++) {
				std::shared_ptr<Transform> t = std::any_cast<std::shared_ptr<Transform>>(collider->getField("transform"));
				glm::vec2 position = std::any_cast<glm::vec2>(t->getField("position"));
				// Set the correct offset
				shapes->at(j).m_centroid = b2Vec2(position.x, position.y);

				b2FixtureDef boxFixtureDef;
				boxFixtureDef.shape = &(shapes->at(j));
				boxFixtureDef.density = 1;
				body->CreateFixture(&boxFixtureDef);
			}
		}

		// Sets the body's initial position and rotation
		body->SetTransform(b2Vec2(position.x, position.y), rotation * Math::deg2rad);
	}

	void Rigidbody2D::update() {
		std::shared_ptr<Transform> transform = std::any_cast<std::shared_ptr<Transform>>(getField("transform"));
		b2Transform t = body->GetTransform();
		transform.get()->setField("position", glm::vec2(t.p.x, t.p.y));
		transform.get()->setField("rotation", t.q.GetAngle());
	}

	void Rigidbody2D::drawFields() {}
}