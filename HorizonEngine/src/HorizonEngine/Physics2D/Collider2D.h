#pragma once

#ifndef HZN_COLLIDER_2D_H
#define HZN_COLLIDER_2D_H

#include <glm/vec2.hpp>

#include <box2d/b2_polygon_shape.h>

#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../GameObject.h"
#include "PhysicsMaterial.h"
#include "ColliderType.h"

namespace Hzn {
	namespace Physics2D {
		class Collider2D : public Component {
		public:
			std::vector<ComponentType> componentTypes;
			std::weak_ptr<Transform> transform;

			glm::vec2 size;
			glm::vec2 offset;
			std::weak_ptr<PhysicsMaterial> material;
			const ColliderType type;
		public:
			virtual std::vector<b2PolygonShape>* generateCollider() = 0;
		private:
			virtual void awake() = 0;
			virtual void start() = 0;
			virtual void update() = 0;
			virtual void fixedUpdate() = 0;
		};
	}
}

#endif