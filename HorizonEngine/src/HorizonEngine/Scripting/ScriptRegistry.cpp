#include "pch.h"
#include "ScriptRegistry.h"
#include "Input.h"
#include <Codes/KeyboardCodes.h>
#include "SceneManagement/Scene.h"
#include "SceneManagement/GameObject.h"
#include "ScriptEngine.h"
#include "box2d/b2_body.h"


#define HZN_ADD_INTERNAL_CALL( name ) mono_add_internal_call("Hzn.InternalCalls::"#name, name)


namespace Hzn
{

	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> s_GameObjectHasComponentFuncs;


	static void Greet()
	{
		std::cout << "Hello called from C++" << std::endl;
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::keyPressed(keycode);
	}

	static bool GameObject_HasComponent(uint32_t gameObjectID, MonoReflectionType* componentType)
	{

		Scene* scene = ScriptEngine::GetSceneContext();
		GameObject gameObject = scene->getGameObjectById(gameObjectID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		return s_GameObjectHasComponentFuncs.at(managedType)(gameObject);

	}

	static MonoObject* GetScriptInstance(uint32_t gameObjectID)
	{
		return ScriptEngine::GetManagedInstance(gameObjectID);
	}

	static void TransformComponent_GetTranslation(uint32_t gameObjectID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GameObject gameObject = scene->getGameObjectById(gameObjectID);

		*outTranslation = gameObject.getComponent<TransformComponent>().m_Translation;
	}

	static void TransformComponent_SetTranslation(uint32_t gameObjectID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GameObject gameObject = scene->getGameObjectById(gameObjectID);

		gameObject.getComponent<TransformComponent>().m_Translation = *translation;
	}

	static void RigidBody2DComponent_ApplyLinearImpulse(uint32_t gameObjectID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GameObject gameObject = scene->getGameObjectById(gameObjectID);

		auto& rb2d = gameObject.getComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(uint32_t gameObjectID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		GameObject gameObject = scene->getGameObjectById(gameObjectID);

		auto& rb2d = gameObject.getComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	void ScriptRegistry::registerFunctions()
	{
		HZN_ADD_INTERNAL_CALL(Greet);
		HZN_ADD_INTERNAL_CALL(Input_IsKeyDown);
		HZN_ADD_INTERNAL_CALL(GameObject_HasComponent);
		HZN_ADD_INTERNAL_CALL(GetScriptInstance);
		HZN_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		HZN_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		HZN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);



	}
}
