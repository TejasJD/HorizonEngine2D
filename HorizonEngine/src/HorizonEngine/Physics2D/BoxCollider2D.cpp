#include "boxCollider2D.h"
#include "../Components/transform.h"
#include <glm/vec2.hpp>

namespace Hzn {
	namespace Physics2D {
		std::vector<b2PolygonShape>* BoxCollider2D::generateCollider() {
			std::vector<b2PolygonShape>* shapes;
			b2PolygonShape polygonShape;
			polygonShape.SetAsBox(
				size.x / 2,
				size.y / 2,
				b2Vec2(0, 0), //(transform.lock()->position.x + offset.x, transform.lock()->position.y + offset.y),
				transform.lock()->rotation
			);
			shapes->push_back(polygonShape);
			return shapes;
		}
	}
}