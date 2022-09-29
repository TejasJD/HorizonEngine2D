#pragma once

#ifndef _rigidbody_2D_h
#define _rigidbody_2D_h

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
		private:
			b2Body* body;
			BodyType type;
			glm::vec2 size;
			glm::vec2 offset;
		public:
			std::vector<ComponentType> componentTypes{ ComponentType::C_Rigidbody2D };
			std::weak_ptr<Transform> transform;
		private:
			Rigidbody2D();
			~Rigidbody2D();
			void init();
		};
	}
}

#endif // !_rigidbody_2D_h