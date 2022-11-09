#pragma once

#ifndef HZN_BOX_COLLIDER_2D
#define HZN_BOX_COLLIDER_2D

#include "Collider2D.h"
#include "../Components/Transform.h"

#include <box2d/box2d.h>
#include <glm/vec2.hpp>

namespace Hzn {
	class BoxCollider2D : public Component {
	public:
		std::map<std::string, std::any>* values;
		/*std::shared_ptr<Transform> transform;

		glm::vec2 size;
		glm::vec2 offset;
		*/
	public:
		BoxCollider2D();
		~BoxCollider2D();

		std::vector<b2PolygonShape>* generateCollider();
		void setField(std::string k, std::any v) override;
		std::any getField(std::string k) override;
		std::string getComponentType() override {
			return "BoxCollider2D";
		}
		std::vector<std::string>* stringify() override;
		std::map<std::string, std::any>* getValues() override;
		void drawFields() override;
	private:
		void awake() override {}
		void start() override {}
		void update() override {}
		void fixedUpdate() override {}
	};
}

#endif