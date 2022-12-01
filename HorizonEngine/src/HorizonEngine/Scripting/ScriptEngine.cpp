#include "pch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptEngine.h"
#include "ScriptRegistry.h"
#include <SceneManagement/Scene.h>

namespace Hzn
{
	struct ScriptData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;

		MonoAssembly* appAssembly = nullptr;
		MonoImage* appAssemblyImage = nullptr;

		Scene* SceneContext = nullptr;
		std::unordered_map<uint32_t, std::shared_ptr<ScriptInstance>> GameObjectInstances;
	};

	ScriptData* ScriptEngine::s_Data = nullptr;

	namespace Utils
	{
		static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = Utils::ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

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

		HZN_DEBUG("Scripting Engine initialized!");
		// function to load the mono C# assembly.
		s_Data->coreAssembly = Utils::LoadCSharpAssembly(path.string());
		s_Data->coreAssemblyImage = mono_assembly_get_image(s_Data->coreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& path)
	{
		s_Data->appAssembly = Utils::LoadCSharpAssembly(path.string());
		s_Data->appAssemblyImage = mono_assembly_get_image(s_Data->appAssembly);
	}



	void ScriptEngine::init()
	{
		s_Data = new ScriptData();
		initMono();
		LoadCoreAssembly("Scripts/ScriptCoreLib.dll");

		ScriptRegistry::registerFunctions();
		// get mono class from the image.
		PrintCoreAssemblyTypes();
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
		mono_domain_unload(s_Data->appDomain);
		s_Data->appDomain = nullptr;
		s_Data->rootDomain = nullptr;
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	MonoObject* ScriptEngine::GetManagedInstance(uint32_t gameObjectID)
	{
		return s_Data->GameObjectInstances.at(gameObjectID)->GetManagedObject();
	}
}