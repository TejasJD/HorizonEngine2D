#pragma once

#ifndef HZN_GAME_OBJECT_H
#define HZN_GAME_OBJECT_H

#include <entt/entt.hpp>
#include "Scene.h"
#include "HorizonEngine/Components/Component.h"

namespace Hzn
{
	class GameObject
	{
		friend class Scene;
		friend struct RelationComponent;
		friend bool operator==(const GameObject& lhs, const GameObject& rhs);
	public:
		// you are capable of initializing an invalid game object.
		// this object is made valid when you assign the return value of
		// createGameObject() member function by the Scene.
		GameObject() = default;
		GameObject(const GameObject& rhs) = default;
		~GameObject() = default;


		operator bool() const { return m_Scene && m_Scene->m_Valid && m_Scene->m_Registry.valid(m_ObjectId); }

		template<typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			isValid();
			m_Scene->m_Registry.emplace<T>(m_ObjectId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() const
		{
			isValid();
			return m_Scene->m_Registry.get<T>(m_ObjectId);
		}

		template<typename T>
		void removeComponent() {
			isValid();
			m_Scene->m_Registry.erase<T>(m_ObjectId);
		}

		template<typename T>
		bool hasComponent() const
		{
			isValid();
			return m_Scene->m_Registry.try_get<T>(m_ObjectId) != nullptr;
		}

		bool isParent(const GameObject& obj) const
		{
			isValid();
			return getParent() == obj;
		}

		bool hasChild(const GameObject& obj) const
		{
			isValid();
			auto list = this->getChildren();
			return std::find(list.begin(), list.end(), obj) != list.end();
		}

		uint32_t getObjectId() const { return entt::to_integral(m_ObjectId); }

		void setParent(GameObject& obj);
		GameObject getParent() const;
		GameObject getNextSibling() const;
		GameObject getPrevSibling() const;
		std::vector<GameObject> getChildren() const;
		size_t getChildCount() const;
		std::vector<GameObject> getChildrenAll() const;
		void addChild(GameObject& obj);
		void removeChild(GameObject& obj);
		bool isAncestorOf(const GameObject& obj) const;
		glm::mat4 getTransform() const;
		GameObject duplicateAsChild();
		GameObject duplicate();
		void destroy();

		template<typename... Component>
		GameObject cloneComponents(ComponentGroup<Component...>)
		{
			isValid();
			GameObject obj{ m_Scene->m_Registry.create(), m_Scene };

			([&]
				{
					if (hasComponent<Component>())
					{
						if (typeid(Component) != typeid(RelationComponent))
						{
							obj.addComponent<Component>(getComponent<Component>());
						}
					}
				}(), ...);
			obj.addComponent<RelationComponent>();
			m_Scene->m_GameObjectIdMap.emplace(entt::to_integral(obj.m_ObjectId), obj.m_ObjectId);
			auto& transformComponent = m_Scene->m_Registry.get<TransformComponent>(obj.m_ObjectId);
			HZN_CORE_INFO("{}, {}, {}", transformComponent.m_Translation.x, transformComponent.m_Translation.y, transformComponent.m_Translation.z);
			return obj;
		}

		void onCollisionEnter(GameObject &other);
		void onCollisionExit(GameObject& other);
		void onTriggerEnter(GameObject& other);
		void onTriggerExit(GameObject& other);


		// The 4 methods below are added for the sake of the game. Delete later!
		void addCollisionEnetrCallback(void (*f)(GameObject));
		void addCollisionExitCallback(std::function<void(GameObject)> &f);
		void addTriggerEnetrCallback(std::function<void(GameObject)> &f);
		void addTriggerExitCallback(std::function<void(GameObject)> &f);

	private:
		// this constructor is used by the Scene to give you a valid game object.
		GameObject(const entt::entity& object, Scene* scene) : m_ObjectId(object), m_Scene(scene) {}

		void isValid() const
		{
			if (!m_Scene || !m_Scene->m_Valid)
			{
				HZN_CORE_ERROR("Game Object invalidated! Doesn't belong to any scene!");
				throw std::runtime_error("Game Object invalidated! Doesn't belong to any scene!");
			}
		}

		// if both scenes are not nullptr and both pointers point to the same scene.
		bool sameScene(const GameObject& rhs) const { return m_Scene && rhs.m_Scene && &*m_Scene == &*rhs.m_Scene; }

		// game object id.
		entt::entity m_ObjectId = entt::null;
		// holds a weak reference to the scene.
		Scene* m_Scene = nullptr;

		std::vector<std::function<void(GameObject)>*> collisionEnterCallbacks;
	};

	inline bool operator==(const GameObject& lhs, const GameObject& rhs)
	{
		return lhs.m_Scene == rhs.m_Scene && lhs.m_ObjectId == rhs.m_ObjectId;
	}

	inline bool operator!=(const GameObject& lhs, const GameObject& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif