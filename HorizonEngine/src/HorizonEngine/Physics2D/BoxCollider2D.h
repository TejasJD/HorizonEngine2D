#pragma once

#ifndef _box_collider_2D_h
#define _box_collider_2D_h

#include "Collider2D.h"
#include "../Components/Transform.h"

#include <box2d/box2d.h>
#include <glm/vec2.hpp>

namespace Hzn {
	namespace Physics2D {
		class BoxCollider2D : public Collider2D {
		public:
			std::vector<ComponentType> componentTypes{ ComponentType::C_Collider2D, ComponentType::C_BoxCollider2D };
			std::weak_ptr<Transform> transform;

			glm::vec2 size;
			glm::vec2 offset;
			std::weak_ptr<PhysicsMaterial> material;
			const ColliderType type;
		public:
			std::vector<b2PolygonShape>* generateCollider();
		private:
			void awake();
			void start();
			void update();
			void fixedUpdate();
		};
	}
}

#endif