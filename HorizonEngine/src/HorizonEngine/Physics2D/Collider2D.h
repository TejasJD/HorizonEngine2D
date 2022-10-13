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
	class Collider2D : public Component {
	public:
		std::map<std::string, std::any>* values;
		/*std::shared_ptr<Transform> transform;

		glm::vec2 size;
		glm::vec2 offset;*/
	public:
		virtual std::string getComponentType() = 0;
		virtual void setField(std::string k, std::any v);
		virtual std::any getField(std::string k);
		virtual std::vector<std::string>* stringify();
		virtual std::vector<b2PolygonShape>* generateCollider() = 0;
	private:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
	};
}

#endif