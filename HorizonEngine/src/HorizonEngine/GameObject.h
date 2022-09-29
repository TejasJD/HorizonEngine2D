#pragma once

#ifndef _game_object_h
#define _game_object_h

#include "HorizonEngine/Components/Transform.h"
#include "HorizonEngine/Components/Component.h"

namespace Hzn {
	class Transform;
	class Component;

	class GameObject {
	public:
		char* name;
		Transform* transform;
	private:
		bool active;
		std::vector<Component*>* components;
	public:
		Component* getComponent(ComponentType type) const;

		std::vector<Component*>* getComponents(ComponentType type) const;

		std::vector<Component*>* getComponentsInChildren(ComponentType type) const;

		void addComponent(Component* component);

		void removeComponent(ComponentType type);
		bool isActive() const {
			return active;
		};
		void setActive(bool active) {
			this->active = active;
		}

		~GameObject() {

		}
	};
}

#endif // !_game_object_h