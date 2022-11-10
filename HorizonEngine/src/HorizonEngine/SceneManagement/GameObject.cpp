#include "pch.h"
#include "HorizonEngine/Components/Component.h"

#include "GameObject.h"


namespace Hzn
{
	GameObject GameObject::getParent() const
	{
		isValid();

		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);
		if (relationComponent.m_Parent != entt::null)
		{
			return { relationComponent.m_Parent, m_Scene };
		}
		else return GameObject();
	}

	GameObject GameObject::getNextSibling() const
	{
		isValid();
		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);
		if (relationComponent.m_Next != entt::null)
		{
			return { relationComponent.m_Next, m_Scene };
		}
		else return GameObject();
	}

	GameObject GameObject::getPrevSibling() const
	{
		isValid();
		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);
		if (relationComponent.m_Prev != entt::null)
		{
			return { relationComponent.m_Prev, m_Scene };
		}
		else return GameObject();
	}

	std::vector<GameObject> GameObject::getChildren() const
	{
		std::vector<GameObject> objects;

		auto& thisObject = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);

		auto curr = thisObject.m_FirstChild;

		while(curr != entt::null)
		{
			objects.push_back({ curr, m_Scene });
			curr = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
		}

		return objects;
	}

	size_t GameObject::getChildCount() const
	{
		isValid();
		return m_Scene->m_Registry.get<RelationComponent>(m_ObjectId).m_ChildCount;
	}

	void GameObject::addChild(const GameObject& obj)
	{
		// if this game object itself is valid.
		isValid();
		// if 'obj' and 'this' are in the same scene.
		if(!sameScene(obj))
		{
			throw std::runtime_error("Objects not in the same scene!");
		}
		// if 'obj' and 'this' are the same objects.
		if (obj == *this) return;

		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);
		auto curr = relationComponent.m_FirstChild;
		auto prev = curr;
		auto size = relationComponent.m_ChildCount;
		if(size == 0)
		{
			relationComponent.m_FirstChild = obj.m_ObjectId;
		}
		else
		{
			while(curr != entt::null)
			{
				prev = curr;
				curr = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
			}
			m_Scene->m_Registry.get<RelationComponent>(prev).m_Next = obj.m_ObjectId;
			m_Scene->m_Registry.get<RelationComponent>(obj.m_ObjectId).m_Prev = prev;
		}

		m_Scene->m_Registry.get<RelationComponent>(obj.m_ObjectId).m_Parent = m_ObjectId;
		relationComponent.m_ChildCount++;
	}

	void GameObject::removeChild(const GameObject& obj)
	{
		isValid();
		if (!sameScene(obj))
		{
			throw std::runtime_error("Objects not in the same scene!");
		}

		if(hasChild(obj))
		{
			auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);
			auto curr = relationComponent.m_FirstChild;
			auto prev = curr;

			while(curr != obj)
			{
				prev = curr;
				curr = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
			}

			if(m_Scene->m_Registry.get<RelationComponent>(curr).m_Next != entt::null)
			{
				auto next = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
				m_Scene->m_Registry.get<RelationComponent>(curr).m_Next = entt::null;
				m_Scene->m_Registry.get<RelationComponent>(next).m_Prev = entt::null;
			}
			
			prev = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
			m_Scene->m_Registry.get<RelationComponent>(obj.m_ObjectId).m_Prev = entt::null;
			m_Scene->m_Registry.get<RelationComponent>(obj.m_ObjectId).m_Parent = entt::null;
			relationComponent.m_ChildCount--;
		}
	}

}
