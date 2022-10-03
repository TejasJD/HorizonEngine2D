#pragma once

#ifndef HZN_RIGID_BODY_2D_H
#define HZN_RIGID_BODY_2D_H

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include "PhysicsWorld2D.h"
#include "BodyType.h"
#include "Collider2D.h"
#include "../Utils/Math.h"
#include "../Components/Component.h"
#include "../Components/ComponentType.h"
#include "../Components/Transform.h"

namespace Hzn {
	namespace Physics2D {
		class Rigidbody2D : public Component {
		public:
			std::vector<ComponentType> componentTypes{ ComponentType::C_Rigidbody2D };
			std::weak_ptr<Transform> transform;
		private:
			b2Body* body;
			BodyType type;
			glm::vec2 size;
			glm::vec2 offset;
		public:
			Rigidbody2D();
			~Rigidbody2D();

			void setPosition(glm::vec2 position);
			void setRotation(float rotation);
		private:
			void init();
			void awake() override {}
			void start() override {}
			void update() override {}
			void fixedUpdate() override {}
		};
	}
}

#endif // !_rigidbody_2D_h