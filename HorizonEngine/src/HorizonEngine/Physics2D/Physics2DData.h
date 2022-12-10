#pragma once

#ifndef HZN_PHYSICS_2D_DATA_H
#define HZN_PHYSICS_2D_DATA_H

#include <glm/glm.hpp>

namespace Hzn
{
	class Physics2DData 
	{
	public:
		static glm::vec2 worldOrigin;
		static glm::vec2 gravity;
		static bool allowSleep;
		static void init();
	};
}

#endif