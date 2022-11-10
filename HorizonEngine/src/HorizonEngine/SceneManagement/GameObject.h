#pragma once

#ifndef HZN_GAME_OBJECT_H
#define HZN_GAME_OBJECT_H

#include <entt/entt.hpp>
#include "Scene.h"

namespace Hzn
{
	class GameObject
	{
		friend class Scene;
		friend class RelationComponent;
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
		bool hasComponent() const
		{
			isValid();
			return m_Scene->m_Registry.try_get<T>(m_ObjectId) != nullptr;
		}

		/*template<typename Archive>
		void load(Archive& ar)
		{
			ar(cereal::make_nvp("objectId", entt::to_integral(m_ObjectId)));
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(cereal::make_nvp("objectId", entt::to_integral(m_ObjectId)));
		}*/

		GameObject getParent() const;
		GameObject getNextSibling() const;
		GameObject getPrevSibling() const;
		std::vector<GameObject> getChildren() const;
		size_t getChildCount() const;

		void addChild(const GameObject& obj);
		void removeChild(const GameObject& obj);

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

	private:
		// this constructor is used by the Scene to give you a valid game object.
		GameObject(const entt::entity& object, Scene* scene) : m_ObjectId(object), m_Scene(scene) {}

		// if both scenes are not nullptr and both pointers point to the same scene.
		bool sameScene(const GameObject& rhs) const { return m_Scene && rhs.m_Scene && &*m_Scene == &*rhs.m_Scene; }

		void isValid() const
		{
			if (!m_Scene || !m_Scene->m_Valid)
			{
				HZN_CORE_ERROR("Game Object invalidated! Doesn't belong to any scene!");
				throw std::runtime_error("Game Object invalidated! Doesn't belong to any scene!");
			}
		}

		// game object id.
		entt::entity m_ObjectId = entt::null;
		// holds a weak reference to the scene.
		Scene* m_Scene = nullptr;
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