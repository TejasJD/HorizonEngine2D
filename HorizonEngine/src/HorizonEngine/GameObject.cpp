#include "GameObject.h"

namespace Hzn 
{
	Component* GameObject::getComponent(ComponentType type) const {
		for (int i = 0; i < components->size(); i++) {
			for (int j = 0; j < components->at(i)->componentTypes.size(); j++) {
				if (components->at(i)->componentTypes[j] == type) {
					return components->at(i);
				}
			}
		}

		return NULL;
	}

	std::vector<Component*>* GameObject::getComponents(ComponentType type) const {
		std::vector<Component*>* comps;
		for (int i = 0; i < components->size(); i++) {
			for (int j = 0; j < components->at(i)->componentTypes.size(); j++) {
				if (components->at(i)->componentTypes[j] == type) {
					comps->push_back(components->at(i));
				}
			}
		}

		return comps;
	}

	std::vector<Component*>* GameObject::getComponentsInChildren(ComponentType type) const {
		if (transform->children->size() == 0) return getComponents(type);

		std::vector<Component*>* comps;
		std::vector<Component*>* tempComps = getComponents(type);
		for (int i = 0; i < tempComps->size(); i++)
			comps->push_back(tempComps->at(i));

		for (int i = 0; i < transform->children->size(); i++) {
			tempComps = transform->children->at(i)->gameObject->getComponentsInChildren(type);
			for (int j = 0; j < tempComps->size(); j++) {
				comps->push_back(tempComps->at(j));
			}
		}

		return comps;
	}

	void GameObject::addComponent(Component* component) {
		//components.push_back(std::shared_ptr<Component>(component));
		components->push_back(component);
		// TODO: add reference to the this object if the component is a transform
		if (typeid(component) == typeid(Transform*)) {
			Transform* t = dynamic_cast<Transform*>(component);
			if (t) {
				transform = t;
				t->gameObject = this;
			}
		}
	}

	void GameObject::removeComponent(ComponentType type) {
		for (int i = 0; i < components->size(); i++) {
			for (int j = 0; j < components->at(i)->componentTypes.size(); j++) {
				if (components->at(i)->componentTypes[j] == type) {
					components->erase(std::next(components->begin(), i));
					return;
				}
			}
		}
	}
}