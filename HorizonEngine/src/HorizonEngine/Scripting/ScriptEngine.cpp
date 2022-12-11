#include "pch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

#include "HorizonEngine/Components/Component.h"

#include "HorizonEngine/SceneManagement/GameObject.h"
#include "HorizonEngine/SceneManagement/Scene.h"
#include "HorizonEngine/SceneManagement/FunctionRegistry.h"

#include "HorizonEngine/FileManagement/ProjectManager.h"

#include "ScriptEngine.h"
#include "ScriptRegistry.h"
#include "SceneManagement/SceneManager.h"

namespace Hzn
{
	struct ScriptData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;
		std::filesystem::path coreAssemblyPath;

		MonoAssembly* appAssembly = nullptr;
		MonoImage* appAssemblyImage = nullptr;
		std::filesystem::path appAssemblyPath;
		bool reloadPending = false;

		std::shared_ptr<ScriptClass> gameObjectClass;
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> gameObjectSubClasses;
		std::unordered_map<uint32_t, std::shared_ptr<ScriptObject>> gameObjectScriptInstances;
	};

	ScriptData* ScriptEngine::s_Data = nullptr;

	namespace Utils
	{
		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_full(assemblyPath.c_str(), &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);


			return assembly;
		}

	}

	// Script Reflection Method Implementations.
	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore)
	{
		if(!isCore)
		{
			m_MonoClass = mono_class_from_name(ScriptEngine::s_Data->appAssemblyImage, nameSpace.c_str(), name.c_str());
		}
		else
		{
			m_MonoClass = mono_class_from_name(ScriptEngine::s_Data->coreAssemblyImage, nameSpace.c_str(), name.c_str());
		}
	}

	MonoObject* ScriptClass::instantiate() const
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::getMethod(const std::string& name, int paramCount) const
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), paramCount);
	}

	MonoObject* ScriptClass::invokeMethod(MonoObject* instance, MonoMethod* method, void** params) const
	{
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}

	ScriptObject::ScriptObject(const std::shared_ptr<ScriptClass>& scriptClass, const GameObject& obj)
		: m_ScriptClass(scriptClass)
	{
		m_Object = m_ScriptClass->instantiate();

		m_Constructor = ScriptEngine::s_Data->gameObjectClass->getMethod(".ctor", 1);
		m_OnCreate= scriptClass->getMethod("OnCreate", 0);
		m_OnUpdate = scriptClass->getMethod("OnUpdate", 1);
		m_OnCollisionEnter = scriptClass->getMethod("OnCollisionEnter", 1);
		m_OnCollisionExit = scriptClass->getMethod("OnCollisionExit", 1);
		m_OnTriggerEnter = scriptClass->getMethod("OnTriggerEnter", 1);
		m_OnTriggerExit = scriptClass->getMethod("OnTriggerExit", 1);

		// invoke game object construtor.
		{
			uint32_t id = obj.getObjectId();
			void* param = &id;
			m_ScriptClass->invokeMethod(m_Object, m_Constructor, &param);
		}

		// here we should add the rigid body contact methods to the constructor.
		if (obj.hasComponent<RigidBody2DComponent>())
		{
			if (m_OnCollisionEnter)
			{
				g_CollisionEnterFunctionMap[obj.getObjectId()] = [&](uint32_t id)
				{
					void* param = &id;
					m_ScriptClass->invokeMethod(m_Object, m_OnCollisionEnter, &param);
				};
			}

			if(m_OnCollisionExit)
			{
				g_CollisionExitFunctionMap[obj.getObjectId()] = [&](uint32_t id)
				{
					void* param = &id;
					m_ScriptClass->invokeMethod(m_Object, m_OnCollisionExit, &param);
				};
			}

			if (m_OnTriggerEnter)
			{
				g_TriggerEnterFunctionMap[obj.getObjectId()] = [&](uint32_t id)
				{
					void* param = &id;
					m_ScriptClass->invokeMethod(m_Object, m_OnTriggerEnter, &param);
				};
			}

			if (m_OnTriggerExit)
			{
				g_TriggerExitFunctionMap[obj.getObjectId()] = [&](uint32_t id)
				{
					void* param = &id;
					m_ScriptClass->invokeMethod(m_Object, m_OnTriggerExit, &param);
				};
			}
		}
	}

	void ScriptObject::invokeOnCreate()
	{
		if(m_OnCreate)
		{
			m_ScriptClass->invokeMethod(m_Object, m_OnCreate, nullptr);
		}
	}

	void ScriptObject::invokeOnUpdate(float ts)
	{
		if(m_OnUpdate)
		{
			void* param = &ts;
			m_ScriptClass->invokeMethod(m_Object, m_OnUpdate, &param);
		}
	}

	void ScriptEngine::PrintCoreAssemblyTypes()
	{
		MonoImage* image = mono_assembly_get_image(s_Data->coreAssembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			HZN_CORE_TRACE("{0}, {1}", nameSpace, name);
		}
	}

	void ScriptEngine::LoadScripts()
	{
		s_Data->gameObjectSubClasses.clear();

		MonoClass* gameObjectClass = mono_class_from_name(s_Data->coreAssemblyImage, "Hzn", "GameObject");

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->appAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->appAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_Data->appAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			HZN_CORE_TRACE("{0}, {1}", nameSpace, name);
			// get info of all the OnUpdate and OnCreate methods.
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_Data->appAssemblyImage, nameSpace, name);

			if (monoClass == gameObjectClass)
				continue;

			bool isGameObject = mono_class_is_subclass_of(monoClass, gameObjectClass, false);
			if (!isGameObject)
				continue;

			std::shared_ptr<ScriptClass> scriptClass = std::make_shared<ScriptClass>(nameSpace, name);
			s_Data->gameObjectSubClasses[fullName] = scriptClass;
		}
	}

	std::shared_ptr<ScriptClass> ScriptEngine::GetGameObjectSubClass(const std::string& name)
	{
		if (s_Data->gameObjectSubClasses.find(name) == s_Data->gameObjectSubClasses.end())
			return nullptr;

		return s_Data->gameObjectSubClasses.at(name);
	}

	std::unordered_map<std::string, std::shared_ptr<ScriptClass>> ScriptEngine::GetGameObjectSubClasses()
	{
		return s_Data->gameObjectSubClasses;
	}

	std::shared_ptr<ScriptObject> ScriptEngine::GetGameObjectScriptInstance(uint32_t id)
	{
		if (s_Data->gameObjectScriptInstances.find(id) == s_Data->gameObjectScriptInstances.end())
			return nullptr;

		return s_Data->gameObjectScriptInstances.at(id);
	}

	bool ScriptEngine::GameObjectSubClassExists(const std::string fullName)
	{
		return s_Data->gameObjectSubClasses.find(fullName) != s_Data->gameObjectSubClasses.end();
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	MonoObject* ScriptEngine::GetManagedInstance(uint32_t id)
	{
		HZN_CORE_ASSERT(s_Data->gameObjectScriptInstances.find(id) != s_Data->gameObjectScriptInstances.end(),
			"Cannot find Script Instance attached to the object");
		return s_Data->gameObjectScriptInstances.at(id)->m_Object;
	}

	void ScriptEngine::OnStop()
	{
		s_Data->gameObjectScriptInstances.clear();
	}

	void ScriptEngine::OnCreateGameObject(const GameObject& obj)
	{
		const auto& scriptComponent = obj.getComponent<ScriptComponent>();
		if(GameObjectSubClassExists(scriptComponent.m_ScriptName))
		{
			uint32_t id = obj.getObjectId();
			std::shared_ptr<ScriptObject> instance = std::make_shared<ScriptObject>(
				s_Data->gameObjectSubClasses[scriptComponent.m_ScriptName], obj);
			s_Data->gameObjectScriptInstances[id] = instance;

			instance->invokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateGameObject(const GameObject& obj, TimeStep ts)
	{
		uint32_t id = obj.getObjectId();
		if (s_Data->gameObjectScriptInstances.find(id) != s_Data->gameObjectScriptInstances.end())
		{
			s_Data->gameObjectScriptInstances[id]->invokeOnUpdate(ts);
		}
		else HZN_CORE_ERROR("Couldn't find script component attached to {}", id);
	}

	void ScriptEngine::OnCollisionEnter(const GameObject& obj)
	{
	}

	void ScriptEngine::OnCollisionExit(const GameObject& obj)
	{
	}

	void ScriptEngine::OnTriggerEnter(const GameObject& obj)
	{
	}

	void ScriptEngine::OnTriggerExit(const GameObject& obj)
	{
	}

	void ScriptEngine::LoadCoreAssembly(const std::filesystem::path& path)
	{
		s_Data->appDomain = mono_domain_create_appdomain("HorizonScriptCoreDomain", nullptr);
		mono_domain_set(s_Data->appDomain, true);

		// function to load the mono C# assembly.
		s_Data->coreAssemblyPath = path;
		std::cout << std::filesystem::absolute(path) << std::endl;
		s_Data->coreAssembly = Utils::LoadCSharpAssembly(path.string());
		s_Data->coreAssemblyImage = mono_assembly_get_image(s_Data->coreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& path)
	{
		s_Data->appAssemblyPath = path;
		std::cout << std::filesystem::absolute(path) << std::endl;
		s_Data->appAssembly = Utils::LoadCSharpAssembly(path.string());
		s_Data->appAssemblyImage = mono_assembly_get_image(s_Data->appAssembly);
	}

	std::filesystem::path ScriptEngine::GetCoreAssemblyPath()
	{
		return s_Data->coreAssemblyPath;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->coreAssemblyImage;
	}

	void ScriptEngine::ReloadAssembly()
	{
		s_Data->gameObjectScriptInstances.clear();
		s_Data->gameObjectSubClasses.clear();

		HZN_CORE_WARN("Reload Starting");
		mono_domain_unload(s_Data->appDomain);
		mono_domain_set(mono_get_root_domain(), false);
		// load core assembly.
		LoadCoreAssembly(s_Data->coreAssemblyPath);
		ScriptRegistry::registerComponents();
		s_Data->gameObjectClass.reset(new ScriptClass("Hzn", "GameObject", true));
		PrintCoreAssemblyTypes();

		auto project = Hzn::ProjectManager::getActiveProject();
		if (project)
		{
			// copy the recompiled dll from bin to temp.
			auto binPath = project->getPath().parent_path().string() + "\\bin\\ScriptAppLib.dll";
			auto loadPath = project->getPath().parent_path().string() + "\\load_target\\ScriptAppLib.dll";

			// bin path needs to exist for app assembly to be reloaded.
			if (std::filesystem::exists(binPath))
			{
				// create directory if load_target doesn't exist.
				if(!std::filesystem::exists(std::filesystem::path(loadPath).parent_path()))
				{
					std::filesystem::create_directory(std::filesystem::path(loadPath).parent_path());
				}

				std::filesystem::copy(binPath, loadPath, std::filesystem::copy_options::overwrite_existing);
				// load app assembly.
				LoadAppAssembly(loadPath);
				LoadScripts();
			}
		}
		HZN_CORE_INFO("Reload Successful!");
		s_Data->reloadPending = false;
	}

	void ScriptEngine::startReload()
	{
		s_Data->reloadPending = true;
	}

	bool ScriptEngine::isReloadPending()
	{
		return s_Data->reloadPending;
	}


	void ScriptEngine::init()
	{
		s_Data = new ScriptData();
		initMono();
		s_Data->coreAssemblyPath = std::filesystem::current_path().string() + "\\Scripts\\ScriptCoreLib.dll";
		LoadCoreAssembly(s_Data->coreAssemblyPath);
		PrintCoreAssemblyTypes();
		ScriptRegistry::registerFunctions();

		s_Data->gameObjectClass = std::make_shared<ScriptClass>("Hzn", "GameObject", true);
		// get mono class from the image.
		/*MonoClass* monoClass = mono_class_from_name(s_Data->coreAssemblyImage, "Hzn", "Main");*/
		ScriptRegistry::registerComponents();

		/*std::shared_ptr<ScriptClass> mainClass = std::make_shared<ScriptClass>("Hzn", "Main", true);
		auto instance = mainClass->instantiate();

		auto method = mainClass->getMethod(".ctor", 0);
		mainClass->invokeMethod(instance, method, nullptr);*/

		//// initialize the object from default constructor.
		//MonoObject* instance = mono_object_new(s_Data->appDomain, monoClass);
		//mono_runtime_object_init(instance);

		//// invoke function with no params.
		//MonoMethod* PrintMessage = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		//mono_runtime_invoke(PrintMessage, instance, nullptr, nullptr);

		//// invoke function with params.
		//MonoMethod* PrintInt = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
		//int value = 7;

		//void* params[1] = { &value };
		//mono_runtime_invoke(PrintInt, instance, params, nullptr);

		//MonoMethod* PrintInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);

		//int value1 = 7, value2 = 8;
		//void* printIntParams[2] = { &value1, &value2 };
		//mono_runtime_invoke(PrintInts, instance, printIntParams, nullptr);

		//MonoString* stringParam = mono_string_new(s_Data->appDomain, "Hello world!");

		//MonoMethod* PrintCustomMessage = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);

		//void* customMessage[1] = { stringParam };

		//mono_runtime_invoke(PrintCustomMessage, instance, customMessage, nullptr);
	}

	void ScriptEngine::destroy()
	{
		destroyMono();
		delete s_Data;
	}

	void ScriptEngine::initMono()
	{
		mono_set_assemblies_path("lib/mono");

		s_Data->rootDomain = mono_jit_init("HorizonScriptRuntime");
		HZN_CORE_ASSERT(s_Data->rootDomain, "Root Domain is null!");
		mono_thread_set_main(mono_thread_current());
	}

	void ScriptEngine::destroyMono()
	{
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->appDomain);
		s_Data->appDomain = nullptr;

		mono_jit_cleanup(s_Data->rootDomain);
		s_Data->rootDomain = nullptr;
	}
}
