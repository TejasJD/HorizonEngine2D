#pragma once

#ifndef HZN_PHYSICS_WORLD_2D_H
#define HZN_PHYSICS_WORLD_2D_H

#include <box2d/b2_world.h>

namespace Hzn {
	class PhysicsWorld2D {
	public:
		static constexpr float gravity = 9.81f;
		static b2World* world;
	};
}

#endif // !_physics_world_2d_h

