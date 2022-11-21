#include "pch.h"
#include "HorizonEngine/Components/Component.h"

#include "GameObject.h"


namespace Hzn
{
	static std::unordered_set<uint32_t> allUnder;

	void GameObject::setParent(GameObject& obj) const 
	{
		isValid();

		//getChildrenAll().contains(obj)

		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);

		if (obj) {
			if (!isAncestorOf(obj)) {
				if (getComponent<Hzn::RelationComponent>().hasParent()) {
					getParent().removeChild(*this);
				}
				obj.addChild(*this);
			}
		}
		else {
			if (getComponent<Hzn::RelationComponent>().hasParent()) {
				getParent().removeChild(*this);
			}
			relationComponent.m_Parent = { entt::null };
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

		if(obj.getParent())
		{
			obj.getParent().removeChild(obj);
		}

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

		glm::mat4 transform = getTransform();
		auto& childTransform = m_Scene->m_Registry.get<TransformComponent>(obj.m_ObjectId);
		transform = glm::inverse(transform) * childTransform.getModelMatrix();

		// decompose the 'transform' and set those to child.
		glm::vec3 translation = glm::vec3(0.0f);
		glm::quat orientation = glm::quat();
		glm::vec3 scale = glm::vec3(0.0f);
		glm::vec3 skew = glm::vec3(0.0f);
		glm::vec4 perspective = glm::vec4(0.0f);
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		// conjugate of the quaternion.
		orientation = glm::conjugate(orientation);
		glm::vec3 rotation = glm::eulerAngles(orientation);
		rotation = glm::radians(rotation);
		// update the translation, orientation and rotation of the child transform.
		childTransform.m_Translation = translation;
		childTransform.m_Rotation = rotation;
		childTransform.m_Scale = scale;
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
			// update the transform of 'obj' to world-space.
			// we get all the ancestors of 'this'.
			glm::mat4 transform = getTransform();
			/*auto& parentTransform = m_Scene->m_Registry.get<TransformComponent>(obj.m_ObjectId);*/
			auto& childTransform = m_Scene->m_Registry.get<TransformComponent>(obj.m_ObjectId);
			transform *= childTransform.getModelMatrix();

			// decompose the 'transform' and set those to child.
			glm::vec3 translation = glm::vec3(0.0f);
			glm::quat orientation = glm::quat();
			glm::vec3 scale = glm::vec3(0.0f);
			glm::vec3 skew = glm::vec3(0.0f);
			glm::vec4 perspective = glm::vec4(0.0f);
			glm::decompose(transform, scale, orientation, translation, skew, perspective);

			// conjugate of the quaternion.
			orientation = glm::conjugate(orientation);
			glm::vec3 rotation = glm::eulerAngles(orientation);
			rotation = glm::radians(rotation);
			// update the translation, orientation and rotation of the child transform.
			childTransform.m_Translation = translation;
			childTransform.m_Rotation = rotation;
			childTransform.m_Scale = scale;

			/*glm::vec4 new_Translation = parentMat * glm::vec4(childTransform.m_Translation, 1.0f);
			childTransform.m_Translation = glm::vec3(new_Translation.x, new_Translation.y, new_Translation.z);

			childTransform.m_Rotation += getRotation();
			if (childTransform.m_Rotation > glm::radians(180.0f)) childTransform.m_Rotation -= glm::radians(360.0f);
			if (childTransform.m_Rotation < glm::radians(-180.0f)) childTransform.m_Rotation += glm::radians(360.0f);

			childTransform.m_Scale *= getScale();*/
			

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

	bool GameObject::isAncestorOf(const GameObject& obj) const
	{
		auto list = getChildrenAll();
		return std::count(list.begin(), list.end(), obj);
	}

	float GameObject::getRotation()
	{
		float rotation = 0.0f;
		/*auto ancestors = getAncestorsAll();
		for (auto ancestor : ancestors)
		{
			auto& transformComponent = m_Scene->m_Registry.get<TransformComponent>(ancestor.m_ObjectId);
			rotation += transformComponent.m_Rotation;
			if (rotation > glm::radians(180.0f)) rotation -= glm::radians(360.0f);
			if (rotation < glm::radians( - 180.0f)) rotation += glm::radians(360.0f);
		}
		rotation += m_Scene->m_Registry.get<TransformComponent>(m_ObjectId).m_Rotation;
		if (rotation > glm::radians(180.0f)) rotation -= glm::radians(360.0f);
		if (rotation < glm::radians(-180.0f)) rotation += glm::radians(360.0f);*/
		return rotation;
	}

	glm::vec3 GameObject::getScale()
	{
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		/*auto ancestors = getAncestorsAll();
		for (auto ancestor : ancestors)
		{
			auto& transformComponent = m_Scene->m_Registry.get<TransformComponent>(ancestor.m_ObjectId);
			scale *= transformComponent.m_Scale;
		}*/
		return scale * m_Scene->m_Registry.get<TransformComponent>(m_ObjectId).m_Scale;
	}

	glm::mat4 GameObject::getTransform() const
	{
		glm::mat4 transform = glm::mat4(1.0f);
		auto ancestors = getAncestorsAll();
		for(auto ancestor : ancestors)
		{
			auto& transformComponent = m_Scene->m_Registry.get<TransformComponent>(ancestor.m_ObjectId);
			transform *= transformComponent.getModelMatrix();
		}
		return transform * m_Scene->m_Registry.get<TransformComponent>(m_ObjectId).getModelMatrix();
	}

	std::vector<GameObject> GameObject::getChildrenAll() const
	{
		std::vector<GameObject> result;

		auto children = getChildren();

		for(auto child : children)
		{
			std::vector<GameObject> childResult = child.getChildrenAll();
			result.emplace_back(child);
			result.insert(result.end(), childResult.begin(), childResult.end());
		}

		return result;
	}

	std::vector<GameObject> GameObject::getAncestorsAll() const
	{
		std::vector<GameObject> ancestors;
		auto ancestor = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId).m_Parent;
		while (ancestor != entt::null)
		{
			ancestors.push_back({ ancestor, m_Scene });
			ancestor = m_Scene->m_Registry.get<RelationComponent>(ancestor).m_Parent;
		}
		return ancestors;
	}
}
