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
	public:
		// you are capable of initializing an invalid game object.
		// this object is made valid when you assign the return value of
		// createGameObject() member function by the Scene.
		GameObject() = default;
		GameObject(const GameObject& rhs) = default;
		~GameObject() = default;


		operator bool() const { return m_Scene && m_Scene->m_Registry.valid(m_ObjectId); }

		template<typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			isValid();
			m_Scene->m_Registry.emplace<T>(m_ObjectId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
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

	private:
		// this constructor is used by the Scene to give you a valid game object.
		GameObject(const entt::entity& object, Scene* scene) : m_ObjectId(object), m_Scene(scene) {}

		void isValid() const
		{
			if (!m_Scene)
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
}

#endif