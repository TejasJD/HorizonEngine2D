#include "pch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "HorizonEngine/FileManagement/ProjectManager.h"
#include "ScriptEngine.h"
#include "ScriptRegistry.h"

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
		bool appAssemblyLoaded = false;
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

	void ScriptEngine::PrintAppAssemblyTypes()
	{
		MonoImage* image = mono_assembly_get_image(s_Data->appAssembly);
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
		s_Data->appAssemblyLoaded = true;
	}

	std::filesystem::path ScriptEngine::GetCoreAssemblyPath()
	{
		return s_Data->coreAssemblyPath;
	}

	void ScriptEngine::ReloadAssembly()
	{
		HZN_CORE_WARN("Reload Starting");
		mono_domain_unload(s_Data->appDomain);
		mono_domain_set(mono_get_root_domain(), false);
		// load core assembly.
		LoadCoreAssembly(s_Data->coreAssemblyPath);
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
				PrintAppAssemblyTypes();
			}
		}
		HZN_CORE_INFO("Reload Successful!");
	}

	bool ScriptEngine::isProjectLoaded()
	{
		return s_Data->appAssemblyLoaded;
	}


	void ScriptEngine::init()
	{
		s_Data = new ScriptData();
		initMono();
		s_Data->coreAssemblyPath = std::filesystem::current_path().string() + "\\Scripts\\ScriptCoreLib.dll";
		LoadCoreAssembly(s_Data->coreAssemblyPath);
		PrintCoreAssemblyTypes();
		ScriptRegistry::registerFunctions();
		// get mono class from the image.
		/*MonoClass* monoClass = mono_class_from_name(s_Data->coreAssemblyImage, "Hzn", "Main");*/

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