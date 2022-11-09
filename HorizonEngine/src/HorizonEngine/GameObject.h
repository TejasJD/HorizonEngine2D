#pragma once

#ifndef HZN_GAME_OBJECT_H
#define HZN_GAME_OBJECT_H

#include "HorizonEngine/Components/Transform.h"
#include "HorizonEngine/Components/Component.h"

namespace Hzn {
	class Transform;
	class Component;
	class Scene;

	class GameObject : public std::enable_shared_from_this<GameObject> {
	public:
		/*friend class Scene;
		Scene* scene = nullptr;*/
		std::string name;
		std::shared_ptr<Transform> transform;
	private:
		bool active;
		std::vector<std::shared_ptr<Component>>* components;
	public:
		GameObject();
		//GameObject(Scene* scene);
		~GameObject();
		GameObject(std::string s);
		//GameObject(Scene* scene, std::string s);
		GameObject(std::shared_ptr<GameObject> gameObject);
		GameObject(GameObject* go);

		std::shared_ptr<Component> getComponent(std::string type) const;

		std::vector<std::shared_ptr<Component>>* getComponents() const;

		std::vector<std::shared_ptr<Component>>* getComponents(std::string type) const;

		std::vector<std::shared_ptr<Component>>* getComponentsInChildren(std::string type) const;

		void addComponent(std::shared_ptr<Component> component);

		void removeComponent(std::string type);
		bool isActive() const {
			return active;
		};
		void setActive(bool active) {
			this->active = active;
		}
	};
}

#endif // !HZN_GAME_OBJECT_H