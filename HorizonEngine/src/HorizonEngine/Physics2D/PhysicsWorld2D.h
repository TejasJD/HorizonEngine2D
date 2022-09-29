#pragma once

#ifndef _physics_world_2d_h
#define _physics_world_2d_h

#include <box2d/b2_world.h>

namespace Hzn {
	namespace Physics2D {
		class PhysicsWorld2D {
		public:
			static constexpr float gravity = 9.81f;
			static b2World* world;
		};
	}
}

#endif // !_physics_world_2d_h

