#pragma once
#include <vector>

#include "../components/transform.h"
#include "../components/component.h"

class Component;
class Transform;

class GameObject {
public:
	char* name;
	Transform* transform;
private:
	bool active;
	std::vector<Component*>* components;
public:
	void getComponent();
	void addComponent();
	void removeComponent();
	bool isActive() {
		return active;
	};
	void setActive(bool active) {
		this->active = active;
	}
};