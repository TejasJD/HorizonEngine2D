#include "pch.h"
#include "HorizonEngine/Components/Component.h"

#include "GameObject.h"


namespace Hzn
{
	void GameObject::setParent(GameObject& obj) const 
	{
		isValid();

		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);

		try {
			obj.isValid();
			obj.addChild(*this);
		} catch (std::runtime_error e) {
			relationComponent.m_Parent = {entt::null};
		}

	}

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
			auto curr = obj.m_ObjectId;
			auto parent = m_ObjectId;
			auto& childRelations = m_Scene->m_Registry.get<RelationComponent>(curr);
			// parent of 'obj' is '*this'.
			// so if previous exists.
			auto prev = childRelations.m_Prev;
			auto next = childRelations.m_Next;

			// meaning if it is not the first child.
			if(prev != entt::null)
			{
				m_Scene->m_Registry.get<RelationComponent>(prev).m_Next = next;
				// meaning its not the last child.
				if(next != entt::null)
				{
					m_Scene->m_Registry.get<RelationComponent>(next).m_Prev = prev; 
				}
			}
			else
			{
				if (next != entt::null)
				{
					m_Scene->m_Registry.get<RelationComponent>(next).m_Prev = prev;
				}
				// if we are here, it means that 'obj' is the first child of its parent ('*this').
				// so we just move the first child of '*this' to the next sibling of 'obj'.
				m_Scene->m_Registry.get<RelationComponent>(parent).m_FirstChild = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
			}

			// break all the relations of 'obj' with the top and the same level of the hierarchy.
			// except with its own children.
			childRelations.m_Parent = entt::null;
			childRelations.m_Next = entt::null;
			childRelations.m_Prev = entt::null;
			// reduce the child count on 'parent'.
			m_Scene->m_Registry.get<RelationComponent>(parent).m_ChildCount--;
		}
	}

}
