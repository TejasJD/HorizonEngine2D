#include "boxCollider2D.h"
#include <glm/vec2.hpp>

namespace Hzn {
	BoxCollider2D::BoxCollider2D() {
		values = new std::map<std::string, std::any>();

		setField("transform", NULL);
		setField("size", glm::vec2(1, 1));
		setField("offset", glm::vec2(0, 0));
	}

	BoxCollider2D::~BoxCollider2D() {

	}

	void BoxCollider2D::setField(std::string k, std::any v) {
		values->insert_or_assign(k, v);
	}

	std::any BoxCollider2D::getField(std::string k) {
		return values->find(k)->second;
	}

	std::vector<std::string>* BoxCollider2D::stringify() {
		std::vector<std::string>* content = new std::vector<std::string>();

		content->push_back("component:" + getComponentType() + "\n");
		content->push_back("values:[\n");
		std::shared_ptr<Component> transform;
		try {
			transform = std::any_cast<std::shared_ptr<Component>>(getField("transform"));
		}
		catch (const std::bad_any_cast& e) {
			transform = NULL;
		}
		if (transform == NULL) {
			std::cout << "NULL" << std::endl;
			content->push_back("\ttransform:Transform:NULL\n");
		}
		else {
			std::shared_ptr<GameObject> go = std::any_cast<std::shared_ptr<GameObject>>(transform->getField("gameObject"));
			content->push_back("\ttransform:Transform:" + go->name + "\n");
		}
		glm::vec2 size = std::any_cast<glm::vec2>(getField("size"));
		content->push_back("\tsize:vec2:" + std::to_string(size.x) + "," + std::to_string(size.y) + "\n");
		glm::vec2 offset = std::any_cast<glm::vec2>(getField("offset"));
		content->push_back("\toffset:vec2:" + std::to_string(offset.x) + "," + std::to_string(offset.y) + "\n");
		content->push_back("]\n");

		return content;
	}

	std::map<std::string, std::any>* BoxCollider2D::getValues() {
		return values;
	}

	std::vector<b2PolygonShape>* BoxCollider2D::generateCollider() {
		std::vector<b2PolygonShape>* shapes;
		b2PolygonShape polygonShape;
		glm::vec2 size = std::any_cast<glm::vec2>(getField("size"));
		std::shared_ptr<Transform> transform = std::any_cast<std::shared_ptr<Transform>>(getField("transform"));
		polygonShape.SetAsBox(
			size.x / 2,
			size.y / 2,
			b2Vec2(0, 0), //(transform.lock()->position.x + offset.x, transform.lock()->position.y + offset.y),
			std::any_cast<float>(transform->getField("rotation"))
		);
		shapes->push_back(polygonShape);
		return shapes;
	}
}