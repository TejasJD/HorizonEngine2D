#pragma once

#ifndef HZN_SCRIPT_ENGINE_H
#define HZN_SCRIPT_ENGINE_H

#include "ScriptReflection.h"

extern "C"
{
	struct _MonoAssembly;
	using MonoAssembly = _MonoAssembly;

	struct _MonoImage;
	using MonoImage = _MonoImage;

	struct _MonoClass;
	using MonoClass = _MonoClass;

	struct _MonoObject;
	using MonoObject = _MonoObject;
}

namespace Hzn
{
	struct ScriptData;

	class ScriptEngine
	{
		friend class ScriptObject;
		friend class ScriptClass;
		friend class ProjectManager;
	public:
		static void init();
		static void destroy();

		static void LoadCoreAssembly(const std::filesystem::path& path);
		static void LoadAppAssembly(const std::filesystem::path& path);

		static std::filesystem::path GetCoreAssemblyPath();
		static MonoImage* GetCoreAssemblyImage();

		static void ReloadAssembly();
		static void PrintCoreAssemblyTypes();
		static void LoadScripts();

		static std::shared_ptr<ScriptClass> GetGameObjectSubClass(const std::string& name);
		static std::unordered_map<std::string, std::shared_ptr<ScriptClass>> GetGameObjectSubClasses();
		static std::shared_ptr<ScriptObject> GetGameObjectScriptInstance(uint32_t id);
		static bool GameObjectSubClassExists(const std::string fullName);
		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static MonoObject* GetManagedInstance(uint32_t id);

		static void OnStop();

		static void OnCreateGameObject(const GameObject& obj);
		static void OnUpdateGameObject(const GameObject& obj, TimeStep ts);
		static void OnCollisionEnter(const GameObject& obj);
		static void OnCollisionExit(const GameObject& obj);
		static void OnTriggerEnter(const GameObject& obj);
		static void OnTriggerExit(const GameObject& obj);

	private:
		static void startReload();
		static bool isReloadPending();

		static void initMono();
		static void destroyMono();
		static ScriptData* s_Data;
	};
}

#endif