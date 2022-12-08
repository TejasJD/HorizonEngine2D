#include "pch.h"

#include <glm/glm.hpp>

#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Codes/KeyboardCodes.h"
#include "HorizonEngine/Input.h"
#include "HorizonEngine/SceneManagement/GameObject.h"

#include "ScriptEngine.h"
#include "ScriptRegistry.h"

#include "box2d/b2_body.h"
#include "SceneManagement/SceneManager.h"


#define HZN_ADD_INTERNAL_CALL( name ) mono_add_internal_call("Hzn.InternalCalls::"#name, name)


namespace Hzn
{
	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> s_EntityHasComponentFuncs;

	static void Greet()
	{
		std::cout << "Hello called from C++" << std::endl;
	}

	static MonoObject* GetScriptInstance(uint32_t id)
	{
		return ScriptEngine::GetManagedInstance(id);
	}

	static bool GameObject_HasComponent(uint32_t id, MonoReflectionType* componentType)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		HZN_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "couldn't find componentType");
		return s_EntityHasComponentFuncs.at(managedType)(obj);
	}

	static void TransformComponent_GetTranslation(uint32_t id, glm::vec3* outTranslation)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		*outTranslation = obj.getComponent<TransformComponent>().m_Translation;
	}

	static void TransformComponent_SetTranslation(uint32_t id, glm::vec3* translation)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		obj.getComponent<TransformComponent>().m_Translation = *translation;
	}

	static void TransformComponent_GetRotation(uint32_t id, glm::vec3* outRotation)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		*outRotation = obj.getComponent<TransformComponent>().m_Rotation;
	}

	static void TransformComponent_SetRotation(uint32_t id, glm::vec3* rotation)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		obj.getComponent<TransformComponent>().m_Rotation = *rotation;
	}

	static void TransformComponent_GetScale(uint32_t id, glm::vec3* outScale)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		*outScale = obj.getComponent<TransformComponent>().m_Scale;
	}

	static void TransformComponent_SetScale(uint32_t id, glm::vec3* scale)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");

		obj.getComponent<TransformComponent>().m_Scale = *scale;
	}

	static void RigidBody2DComponent_GetLinearVelocity(uint32_t id, glm::vec2* velocity)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		GameObject obj = scene->getGameObjectById(id);
		
		b2Body* body = (b2Body*)obj.getComponent<RigidBody2DComponent>().m_RuntimeBody;
		auto b2Velocity = body->GetLinearVelocity();

		*velocity = { b2Velocity.x, b2Velocity.y };
	}

	static void RigidBody2DComponent_SetLinearVelocity(uint32_t id, glm::vec2* velocity)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		GameObject obj = scene->getGameObjectById(id);

		b2Body* body = (b2Body*)obj.getComponent<RigidBody2DComponent>().m_RuntimeBody;
		body->SetLinearVelocity(b2Vec2{velocity->x, velocity->y});
	}

	static void RigidBody2DComponent_GetAngle(uint32_t id, float* angle)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		GameObject obj = scene->getGameObjectById(id);

		b2Body* body = (b2Body*)obj.getComponent<RigidBody2DComponent>().m_RuntimeBody;
		*angle = body->GetAngle();
	}

	static void RigidBody2DComponent_ApplyLinearImpulse(uint32_t id, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");
		auto& rb2d = obj.getComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(uint32_t id, glm::vec2* impulse, bool wake)
	{
		auto scene = Hzn::SceneManager::getActiveScene();
		HZN_CORE_ASSERT(scene != nullptr, "no scene active!");
		GameObject obj = scene->getGameObjectById(id);
		HZN_CORE_ASSERT(obj, "No obj with this Id exists");
		auto& rb2d = obj.getComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static bool Input_IsKeyDown(Hzn::KeyCode keyCode)
	{
		return Input::keyPressed(keyCode);
	}

	void ScriptRegistry::registerFunctions()
	{
		HZN_ADD_INTERNAL_CALL(Greet);
		HZN_ADD_INTERNAL_CALL(Input_IsKeyDown);
		HZN_ADD_INTERNAL_CALL(GetScriptInstance);
		HZN_ADD_INTERNAL_CALL(GameObject_HasComponent);
		HZN_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		HZN_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		HZN_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		HZN_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		HZN_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		HZN_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_GetLinearVelocity);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_SetLinearVelocity);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_GetAngle);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Hzn.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					HZN_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](GameObject obj) { return obj.hasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptRegistry::registerComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}
}
