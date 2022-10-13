#pragma once
#ifndef HZN_TRANSFORM_H
#define HZN_TRANSFORM_H

#include "ComponentFactory.h"
#include "Component.h"
#include "HorizonEngine/GameObject.h"
#include "HorizonEngine/Utils/Time.h"
#include "HorizonEngine/Utils/Math.h"
#include "glm/vec2.hpp"

namespace Hzn {

	class Transform;
	class GameObject;

	class Transform : public Component {
	public:
		std::map<std::string, std::any>* values;
		/*
		std::shared_ptr<GameObject> gameObject;

		glm::vec2 position = glm::vec2(0, 0);
		float rotation = 0;
		glm::vec2 scale = glm::vec2(1, 1);

		glm::vec2 right = glm::vec2(1, 0);
		glm::vec2 up = glm::vec2(0, 1);

		std::shared_ptr<Transform> parent = NULL;
		std::shared_ptr<Transform> root = NULL;
		int siblingIndex = 0;
		int childrenCount = 0;

		std::vector<std::shared_ptr<Transform>>* children{};
		*/
	public:
		Transform();
		~Transform();
		std::string getComponentType() override {
			return "Transform";
		}
		void setField(std::string k, std::any v) override;
		std::any getField(std::string k) override;
		std::vector<std::string>* stringify() override;
	private:
		void awake() override;
		void start() override;
		void update() override;
		void fixedUpdate() override;
	public:
		// Moves the transform in direction direction by delta units
		void move(glm::vec2 direction, float delta);
		// Moves the transform towards position by delta units
		void moveTowards(glm::vec2 position, float delta);

		// Sets the transform's position to position
		void translate(glm::vec2 position);
		// Sets the transform's position to vec2(x, y)
		void translate(float x, float y);

		// Rotates the transform degrees amount degrees
		void rotate(float angle);

		// Rotates the transform so that the forward vec2 is pointing towards position
		void lookAt(glm::vec2 targetPosition);
		// Rotates the transform so that the forward vec2 is pointing towards target transform
		void lookAt(std::shared_ptr<Transform> target);


		// void detachChildren();
		std::shared_ptr<Transform> getChildByName(std::string name);
		std::shared_ptr<Transform> getChildByIndex(int index);
		bool isChildOf(std::string name);
		void setAsFirstSibling();
		void setAsLastSibling();
		void setSiblingIndex(int newSiblingIndex);
	};
}

#endif
