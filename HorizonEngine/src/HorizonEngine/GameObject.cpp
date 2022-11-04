#include "GameObject.h"
#include <memory>

namespace Hzn 
{
	GameObject::GameObject() {
		components = new std::vector<std::shared_ptr<Component>>();
		name = "GameObject";
	}

	GameObject::GameObject(std::string s) {
		components = new std::vector<std::shared_ptr<Component>>();
		name = s;
	}

	/*GameObject::GameObject(GameObject* gameObject) {
		name = gameObject->name;
		transform = gameObject->transform;
		active = gameObject->isActive();
		components = gameObject->getComponents();
	}*/

	GameObject::GameObject(std::shared_ptr<GameObject> gameObject) {
		name = gameObject->name;
		transform = gameObject->transform;
		active = gameObject->isActive();
		components = gameObject->getComponents();
	}

	GameObject::~GameObject() {

	}

	std::shared_ptr<Component> GameObject::getComponent(std::string type) const {
		for (int i = 0; i < components->size(); i++) {
			if (components->at(i)->getComponentType().compare(type) == 0) {
				return components->at(i);
			}
		}

		return NULL;
	}

	std::vector<std::shared_ptr<Component>>* GameObject::getComponents() const {
		return components;
	}

	std::vector<std::shared_ptr<Component>>* GameObject::getComponents(std::string type) const {
		std::vector<std::shared_ptr<Component>>* comps;
		for (int i = 0; i < components->size(); i++) {
			if (components->at(i)->getComponentType() == type) {
				comps->push_back(components->at(i));
			}
		}

		return comps;
	}

	std::vector<std::shared_ptr<Component>>* GameObject::getComponentsInChildren(std::string type) const {
		std::vector<std::shared_ptr<Transform>>* children = std::any_cast<std::vector<std::shared_ptr<Transform>>*>(transform->getField("children"));
		if (children->size() == 0) return getComponents(type);

		std::vector<std::shared_ptr<Component>>* comps{};
		std::vector<std::shared_ptr<Component>>* tempComps = getComponents(type);
		for (int i = 0; i < tempComps->size(); i++)
			comps->push_back(tempComps->at(i));

		for (int i = 0; i < children->size(); i++) {
			std::shared_ptr gameObject = std::any_cast<std::shared_ptr<GameObject>>(children->at(i)->getField("gameObject"));
			tempComps = gameObject->getComponentsInChildren(type);
			for (int j = 0; j < tempComps->size(); j++) {
				comps->push_back(tempComps->at(j));
			}
		}

		return comps;
	}

	void GameObject::addComponent(std::shared_ptr<Component> component) {
		//components.push_back(std::shared_ptr<Component>(component));
		components->push_back(std::shared_ptr<Component>(component));

		if (component->getComponentType().compare("Transform") == 0) {
			std::shared_ptr<Transform> t = std::dynamic_pointer_cast<Transform>(std::shared_ptr<Component>(component));
			if (t) {
				transform = t;
				t->setField("gameObject", shared_from_this());
				t->setField("parent", std::shared_ptr<Component>(component));
				t->setField("root", std::shared_ptr<Component>(component));
			}
		}
		else {
			component->setField("gameObject", shared_from_this());
			component->setField("transform", std::shared_ptr<Component>(transform));
		}
	}

	void GameObject::removeComponent(std::string type) {
		if (type.compare("Transform") == 0) return;

		for (int i = 0; i < components->size(); i++) {
			if (components->at(i)->getComponentType().compare(type) == 0) {
				components->erase(std::next(components->begin(), i));
				return;
			}
		}
	}
}