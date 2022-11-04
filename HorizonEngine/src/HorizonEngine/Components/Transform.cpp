#include "pch.h"
#include "Transform.h"

namespace Hzn {

	Transform::Transform() {
		// TODO: Attach awake, start update and fixedUpdate 
		// to main game loop

		values = new std::map<std::string, std::any>();

		setField("gameObject", NULL);
		setField("position", glm::vec2(0, 0));
		setField("rotation", 0.0f);
		setField("scale", glm::vec2(1, 1));
		setField("right", glm::vec2(1, 0));
		setField("up", glm::vec2(0, 1));
		setField("parent", NULL);
		setField("root", NULL);
		setField("siblingIndex", 0);
		setField("childrenCount", 0);
		setField("children", new std::vector<std::shared_ptr<Transform>>());
	}

	Transform::~Transform() {

	}

	void Transform::setField(std::string k, std::any v) {
		values->insert_or_assign(k, v);
	}

	std::any Transform::getField(std::string k) {
		return values->find(k)->second;
	}

	std::vector<std::string>* Transform::stringify() {
		std::vector<std::string>* content = new std::vector<std::string>();

		content->push_back("component:" + getComponentType() + "\n");
		content->push_back("values:[\n");
		glm::vec2 position = std::any_cast<glm::vec2>(getField("position"));
		content->push_back("\tposition:vec2:" + std::to_string(position.x) + "," + std::to_string(position.y) + "\n");
		float rotation = std::any_cast<float>(getField("rotation"));
		content->push_back("\trotation:float:" + std::to_string(rotation) + "\n");
		glm::vec2 scale = std::any_cast<glm::vec2>(getField("scale"));
		content->push_back("\tscale:vec2:" + std::to_string(scale.x) + "," + std::to_string(scale.y) + "\n");
		glm::vec2 right = std::any_cast<glm::vec2>(getField("right"));
		content->push_back("\tright:vec2:" + std::to_string(right.x) + "," + std::to_string(right.y) + "\n");
		glm::vec2 up = std::any_cast<glm::vec2>(getField("up"));
		content->push_back("\tup:vec2:" + std::to_string(up.x) + "," + std::to_string(up.y) + "\n");
		std::shared_ptr<Component> parent;
		try {
			parent = std::any_cast<std::shared_ptr<Component>>(getField("parent"));
		}
		catch (const std::bad_any_cast& e) {
			parent = NULL;
		}
		if (parent == NULL) {
			content->push_back("\tparent:Transform:NULL\n");
		}
		else {
			std::shared_ptr<GameObject> go = std::any_cast<std::shared_ptr<GameObject>>(parent->getField("gameObject"));
			content->push_back("\tparent:Transform:" + go->name + "\n");
		}
		std::shared_ptr<Component> root;
		try {
			root = std::any_cast<std::shared_ptr<Component>>(getField("root"));
		}
		catch (const std::bad_any_cast& e) {
			root = NULL;
		}
		if (root == NULL) {
			content->push_back("\troot:Transform:NULL\n");
		}
		else {
			std::shared_ptr<GameObject> go = std::any_cast<std::shared_ptr<GameObject>>(root->getField("gameObject"));
			content->push_back("\troot:Transform:" + go->name + "\n");
		}
		int siblingIndex = std::any_cast<int>(getField("siblingIndex"));
		content->push_back("\tsiblingIndex:int:" + std::to_string(siblingIndex) + "\n");
		int childCount = std::any_cast<int>(getField("childrenCount"));
		content->push_back("\tchildrenCount:int:" + std::to_string(childCount) + "\n");
		content->push_back("]\n");

		return content;
	}

	std::map<std::string, std::any>* Transform::getValues() {
		return values;
	}

	void Transform::awake() {}

	void Transform::start() {}

	void Transform::update() {}

	void Transform::fixedUpdate() {}

	void Transform::move(glm::vec2 direction, float delta) {
		setField("position", std::any_cast<glm::vec2>(getField("position")) + direction * delta);
	}

	void Transform::moveTowards(glm::vec2 targetPosition, float delta) {
		glm::vec2 position = std::any_cast<glm::vec2>(getField("position"));
		glm::vec2 direction = glm::vec2(targetPosition.x - position.x,
			targetPosition.y - position.y);
		direction = direction * delta;
		position = glm::vec2(position.x + direction.x, position.y + direction.y);
		setField("position", position);
	}

	void Transform::translate(glm::vec2 translation) {
		glm::vec2 position = std::any_cast<glm::vec2>(getField("position"));
		position = glm::vec2(position.x + translation.x, position.y + translation.y);
		setField("position", position);
	}

	void Transform::translate(float x, float y) {
		translate(glm::vec2(x, y));
	}

	void Transform::rotate(float angle) {
		float rotation = std::any_cast<float>(getField("rotation"));
		rotation += angle;
		rotation = std::fmod(rotation, 360.0f);
		setField("rotation", rotation);

		glm::vec2 rotateVector = glm::vec2(std::cosf(angle), std::sinf(angle));
		glm::vec2 rotateVector90 = glm::vec2(-std::sinf(angle), std::cosf(angle));

		glm::vec2 right = std::any_cast<glm::vec2>(getField("right"));
		right = right.x * rotateVector + right.y * rotateVector90;
		setField("right", right);

		glm::vec2 up = std::any_cast<glm::vec2>(getField("up"));
		up = up.x * rotateVector + up.y * rotateVector90;
		setField("up", up);
	}

	void Transform::lookAt(glm::vec2 targetPosition) {
		glm::vec2 right = std::any_cast<glm::vec2>(getField("right"));
		glm::vec2 up = std::any_cast<glm::vec2>(getField("up"));

		float lengthRight = std::sqrtf((right.x * right.x + right.y * right.y));
		float lengthTraget = std::sqrtf((targetPosition.x * targetPosition.x + targetPosition.y * targetPosition.y));
		float cos = (up.x * targetPosition.x + up.y * targetPosition.y) / (lengthRight * lengthTraget);
		float angle = std::acosf(cos) * 180.0f / Math::PI;
		rotate(angle);
	}

	void Transform::lookAt(std::shared_ptr<Transform> target) {
		lookAt(std::any_cast<glm::vec2>(target->getField("position")));
	}

	std::shared_ptr<Transform> Transform::getChildByName(std::string name) {
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(getField("children"));
		for (int i = 0; i < children->size(); i++) {
			if (std::any_cast<std::shared_ptr<GameObject>>(children->at(i)->getField("gameObject"))->name.compare(name) == 0) {
				return children->at(i);
			}
		}

		return NULL;
	}

	std::shared_ptr<Transform> Transform::getChildByIndex(int index) {
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(getField("children"));
		if (index < children->size())
			return children->at(index);
		return NULL;
	}

	bool Transform::isChildOf(std::string name) {
		std::shared_ptr<Transform> currentTransform(this);
		std::shared_ptr<Transform> parent = std::any_cast<std::shared_ptr<Transform>>(currentTransform->getField("parent"));
		while (parent != NULL) {
			std::shared_ptr<GameObject> gameObject = std::any_cast<std::shared_ptr<GameObject>>(parent->getField("gameObject"));
			if (gameObject->name.compare(name) == 0) {
				return true;
			}
		}

		return false;
	}

	void Transform::setAsFirstSibling() {
		std::shared_ptr<Transform> parent = std::any_cast<std::shared_ptr<Transform>>(getField("parent"));
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(parent->getField("children"));
		int siblingIndex = std::any_cast<int>(getField("siblingIndex"));
		if (parent != NULL) {
			children->erase(children->begin() + siblingIndex);
			children->insert(children->begin(), std::shared_ptr<Transform>(this));
			setField("siblingIndex", 0);
		}
	}

	void Transform::setAsLastSibling() {
		std::shared_ptr<Transform> parent = std::any_cast<std::shared_ptr<Transform>>(getField("parent"));
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(parent->getField("children"));
		int siblingIndex = std::any_cast<int>(getField("siblingIndex"));
		children->erase(children->begin() + siblingIndex);
		children->push_back(std::shared_ptr<Transform>(this));
		setField("siblingIndex", children->size() - 1);
	}

	void Transform::setSiblingIndex(int newSiblingIndex) {
		std::shared_ptr<Transform> parent = std::any_cast<std::shared_ptr<Transform>>(getField("parent"));
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(parent->getField("children"));
		int siblingIndex = std::any_cast<int>(getField("siblingIndex"));
		children->erase(children->begin() + siblingIndex);
		children->insert(children->begin() + newSiblingIndex, std::shared_ptr<Transform>(this));
		setField("siblingIndex", newSiblingIndex);
		for (int i = newSiblingIndex + 1; i < children->size(); i++) {
			children->at(i)->setSiblingIndex(std::any_cast<int>(children->at(i)->getField("siblingIndex")) + 1);
		}
	}
}