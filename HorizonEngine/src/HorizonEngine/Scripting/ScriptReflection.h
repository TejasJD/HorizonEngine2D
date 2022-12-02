#pragma once

#ifndef HZN_SCRIPT_REFLECTION_H
#define HZN_SCRIPT_REFLECTION_H

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

	struct _MonoMethod;
	using MonoMethod = _MonoMethod;
}


namespace Hzn
{
	class ScriptEngine;
	class ScriptClass;
	class GameObject;

	class ScriptObject
	{
		friend class ScriptEngine;
		friend class ScriptClass;
	public:
		ScriptObject(const std::shared_ptr<ScriptClass>& scriptClass, const GameObject& obj);
		~ScriptObject() = default;
		MonoObject* getObject() { return m_Object; }
		void invokeOnCreate();
		void invokeOnUpdate(float ts);

	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;

		MonoObject* m_Object = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreate = nullptr;
		MonoMethod* m_OnUpdate = nullptr;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore = false);

		MonoObject* instantiate() const;
		MonoMethod* getMethod(const std::string& name, int paramCount) const;
		MonoObject* invokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr) const;

		~ScriptClass() = default;
	private:
		std::string m_NameSpace;
		std::string m_Name;

		MonoClass* m_MonoClass = nullptr;
	};
}

#endif