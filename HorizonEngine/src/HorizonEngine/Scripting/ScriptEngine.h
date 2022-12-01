#pragma once

#ifndef HZN_SCRIPT_ENGINE_H
#define HZN_SCRIPT_ENGINE_H

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

extern "C"
{
	struct _MonoAssembly;
	using MonoAssembly = _MonoAssembly;

	struct _MonoImage;
	using MonoImage = _MonoImage;
}

namespace Hzn
{
	struct ScriptData;

	class ScriptClass
	{
		
	};

	class ScriptInstance
	{
		friend class GameObject;
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, GameObject gameObject);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		std::shared_ptr<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			SetFieldValueInternal(name, &value);
		}

		MonoObject* GetManagedObject() { return m_Instance; }
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static char s_FieldValueBuffer[16];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	};


	class ScriptEngine
	{
		friend class Scene;
	public:
		static void init();
		static void destroy();

		static void LoadCoreAssembly(const std::filesystem::path& path);
		static void LoadAppAssembly(const std::filesystem::path& path);
		static void PrintCoreAssemblyTypes();
		static void PrintAppAssemblyTypes();
		static Scene* GetSceneContext();
		static MonoObject* GetManagedInstance(uint32_t uuid);
	private:
		static void initMono();
		static void destroyMono();
		static ScriptData* s_Data;
	};
}

#endif