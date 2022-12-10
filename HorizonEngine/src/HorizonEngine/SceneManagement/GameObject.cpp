#include "pch.h"
#include "HorizonEngine/Components/Component.h"

#include "GameObject.h"


namespace Hzn
{
	static std::unordered_set<uint32_t> allUnder;

	void GameObject::setParent(GameObject& obj)
	{
		isValid();

		//getChildrenAll().contains(obj)

		auto& relationComponent = m_Scene->m_Registry.get<RelationComponent>(m_ObjectId);

		if (obj) {
			auto& objrelationComponent = m_Scene->m_Registry.get<RelationComponent>(obj.m_ObjectId);
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

	void GameObject::addChild(GameObject& obj)
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
		
		glm::vec3 rotation = glm::eulerAngles(orientation);
		rotation = glm::degrees(rotation);
		// update the translation, orientation and rotation of the child transform.
		childTransform.m_Translation = translation;
		childTransform.m_Rotation = rotation;
		childTransform.m_Scale = scale;
	}

	void GameObject::removeChild(GameObject& obj)
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

			glm::vec3 rotation = glm::eulerAngles(orientation);
			rotation = glm::degrees(rotation);
			// update the translation, orientation and rotation of the child transform.
			childTransform.m_Translation = translation;
			childTransform.m_Rotation = rotation;
			childTransform.m_Scale = scale;
			

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
		isValid();
		auto list = getChildrenAll();
		return std::count(list.begin(), list.end(), obj);
	}

	glm::mat4 GameObject::getTransform() const
	{
		if (getParent()) return getParent().getTransform() * m_Scene->m_Registry.get<TransformComponent>(m_ObjectId).getModelMatrix();
		return glm::mat4(1.0f) * m_Scene->m_Registry.get<TransformComponent>(m_ObjectId).getModelMatrix();
	}

	GameObject GameObject::duplicateAsChild()
	{
		isValid();
		auto clonedObject = duplicate();
		auto& clonedRel = m_Scene->m_Registry.get<RelationComponent>(clonedObject.m_ObjectId);
		auto& originalParentRel = getParent().getComponent<RelationComponent>();
		originalParentRel.m_ChildCount++;
		auto curr = originalParentRel.m_FirstChild;
		auto prev = curr;

		while(curr != entt::null)
		{
			prev = curr;
			curr = m_Scene->m_Registry.get<RelationComponent>(curr).m_Next;
		}
		clonedRel.m_Prev = prev;
		m_Scene->m_Registry.get<RelationComponent>(prev).m_Next = clonedObject.m_ObjectId;
		clonedRel.m_Parent = getParent().m_ObjectId;
		return clonedObject;
	}

	GameObject GameObject::duplicate()
	{
		isValid();
		GameObject clonedParent = cloneComponents(AllComponents{});
		auto& parentRel = clonedParent.getComponent<RelationComponent>();
		auto prev = parentRel.m_FirstChild;

		auto children = getChildren();

		for(auto& child : children)
		{
			GameObject clonedChild = child.duplicate();
			auto& childRel = clonedChild.getComponent<RelationComponent>();

			if(prev == entt::null)
			{
				parentRel.m_FirstChild = clonedChild.m_ObjectId;
			}
			else
			{
				childRel.m_Prev = prev;
				m_Scene->m_Registry.get<RelationComponent>(prev).m_Next = clonedChild.m_ObjectId;
				
			}
			prev = clonedChild.m_ObjectId;
			childRel.m_Parent = clonedParent.m_ObjectId;
			parentRel.m_ChildCount++;
		}

		return clonedParent;
	}

	/*void GameObject::destroy() {
		isValid();

		uint32_t id = entt::to_integral(m_ObjectId);
		if (std::find(m_Scene->m_ObjectsToDelete.begin(), m_Scene->m_ObjectsToDelete.end(), id) == m_Scene->m_ObjectsToDelete.end())
			m_Scene->m_ObjectsToDelete.push_back(id);
	}*/

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

	void GameObject::onCollisionEnter(GameObject& other) {
		HZN_CORE_DEBUG(other.getComponent<NameComponent>().m_Name);

		/*for (int i = 0; i < collisionEnterCallbacks.size(); i++) {
			(*collisionEnterCallbacks.at(i))(other);
		}*/
	}

	void GameObject::onCollisionExit(GameObject& other) {
		HZN_CORE_DEBUG(other.getComponent<NameComponent>().m_Name);
	}

	void GameObject::onTriggerEnter(GameObject& other) {
		HZN_CORE_DEBUG(other.getComponent<NameComponent>().m_Name);
	}

	void GameObject::onTriggerExit(GameObject& other) {
		HZN_CORE_DEBUG(other.getComponent<NameComponent>().m_Name);
	}

	void GameObject::addCollisionEnetrCallback(void (*f)(GameObject))
	{
		collisionEnterCallbacks.push_back((std::function<void(GameObject)>*) f);
	}

	void GameObject::addCollisionExitCallback(std::function<void(GameObject)> &f)
	{

	}

	void GameObject::addTriggerEnetrCallback(std::function<void(GameObject)> &f)
	{

	}

	void GameObject::addTriggerExitCallback(std::function<void(GameObject)> &f)
	{

	}
}
