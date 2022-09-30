#include "pch.h"
#include "Transform.h"

namespace Hzn {

	Transform::Transform() {
		// TODO: Attach awake, start update and fixedUpdate 
		// to main game loop

		// TODO: set initial values
		// TODO: set up and right vectors
	}

	Transform::~Transform() {

	}

	void Transform::awake() {}

	void Transform::start() {}

	void Transform::update() {}

	void Transform::fixedUpdate() {}

	void Transform::move(glm::vec2 direction, float delta) {
		position += direction * delta;
	}

	void Transform::moveTowards(glm::vec2 targetPosition, float delta) {
		glm::vec2 direction = glm::vec2(targetPosition.x - this->position.x,
			targetPosition.y - this->position.y);
		direction = direction * delta;
		this->position = glm::vec2(position.x + direction.x,
			position.y + direction.y);
		this->localPosition = glm::vec2(localPosition.x + direction.x,
			localPosition.y + direction.y);
	}

	void Transform::translate(glm::vec2 translation) {
		this->position = glm::vec2(this->position.x + translation.x,
			this->position.y + translation.y);
		this->localPosition = glm::vec2(this->localPosition.x + translation.x,
			this->localPosition.y + translation.y);
	}

	void Transform::translate(float x, float y) {
		translate(glm::vec2(x, y));
	}

	void Transform::rotate(float angle) {
		rotation += angle;
		rotation = std::fmod(rotation, 360.0f);

		glm::vec2 rotateVector = glm::vec2(std::cosf(angle), std::sinf(angle));
		glm::vec2 rotateVector90 = glm::vec2(-std::sinf(angle), std::cosf(angle));
		right = right.x * rotateVector + right.y * rotateVector90;
		up = up.x * rotateVector + up.y * rotateVector90;
	}

	void Transform::lookAt(glm::vec2 targetPosition) {
		float lengthRight = std::sqrtf((right.x * right.x + right.y * right.y));
		float lengthTraget = std::sqrtf((targetPosition.x * targetPosition.x + targetPosition.y * targetPosition.y));
		float cos = (up.x * targetPosition.x + up.y * targetPosition.y) / (lengthRight * lengthTraget);
		float angle = std::acosf(cos) * 180.0f / Math::PI;
		rotate(angle);
	}

	void Transform::lookAt(Transform* target) {
		lookAt(target->position);
	}

	Transform* Transform::getChildByName(char* name) {
		for (int i = 0; i < children->size(); i++) {
			if (strcmp(children->at(i)->gameObject->name, name) == 0) {
				return children->at(i);
			}
		}

		return NULL;
	}

	Transform* Transform::getChildByIndex(int index) {
		if (index < children->size())
			return children->at(index);
		return NULL;
	}

	bool Transform::isChildOf(char* name) {
		Transform* currentTransform = this;
		while (currentTransform->parent.lock() != NULL) {
			if (strcmp(currentTransform->parent.lock()->gameObject->name, name) == 0) {
				return true;
			}
		}

		return false;
	}

	void Transform::setAsFirstSibling() {
		if (parent.lock() != NULL) {
			parent.lock()->children->erase(parent.lock()->children->begin() + siblingIndex);
			parent.lock()->children->insert(parent.lock()->children->begin(), this);
			siblingIndex = 0;
		}
	}

	void Transform::setAsLastSibling() {
		parent.lock()->children->erase(parent.lock()->children->begin() + siblingIndex);
		parent.lock()->children->push_back(this);
		siblingIndex = parent.lock()->children->size();
	}

	void Transform::setSiblingIndex(int newSiblingIndex) {
		parent.lock()->children->erase(parent.lock()->children->begin() + siblingIndex);
		parent.lock()->children->insert(parent.lock()->children->begin() + newSiblingIndex, this);
		siblingIndex = newSiblingIndex;
	}
}