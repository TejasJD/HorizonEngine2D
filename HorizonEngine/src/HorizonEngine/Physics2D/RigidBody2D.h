#pragma once

#ifndef HZN_RIGID_BODY_2D_H
#define HZN_RIGID_BODY_2D_H

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include "PhysicsWorld2D.h"
#include "BodyType.h"
#include "BoxCollider2D.h"
#include "../Utils/Math.h"
#include "../Components/Component.h"
#include "../Components/ComponentType.h"
#include "../Components/Transform.h"

namespace Hzn {
	class Rigidbody2D : public Component {
	public:
		std::map<std::string, std::any>* values;

		/*BodyType type;
		glm::vec2 size;
		glm::vec2 offset;
		std::shared_ptr<Transform> transform;*/
	private:
		b2Body* body;
	public:
		Rigidbody2D();
		~Rigidbody2D();

		std::string getComponentType() override {
			return "Rigidbody2D";
		}
		void setField(std::string k, std::any v) override;
		std::any getField(std::string k) override;
		std::vector<std::string>* stringify() override;
		std::map<std::string, std::any>* getValues() override;
		void drawFields() override;

		void setPosition(glm::vec2 position);
		void setRotation(float rotation);
	private:
		void init();
		void awake() override {}
		void start() override {}
		void update() override;
		void fixedUpdate() override {}
	};
}

#endif // !_rigidbody_2D_h