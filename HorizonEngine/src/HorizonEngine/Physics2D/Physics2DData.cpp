#include "pch.h"

#include "Physics2DData.h"

namespace Hzn {
	glm::vec2 Physics2DData::worldOrigin{ 0.0f, 0.0f };
	glm::vec2 Physics2DData::gravity{ 0.0f, 9.81f };
	bool Physics2DData::allowSleep = true;

	void Physics2DData::init() {
		// Maybe read from project file?
	}
}