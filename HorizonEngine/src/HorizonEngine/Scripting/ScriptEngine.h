#pragma once

#ifndef HZN_SCRIPT_ENGINE_H
#define HZN_SCRIPT_ENGINE_H

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

	class ScriptEngine
	{
	public:
		static void init();
		static void destroy();

		static void LoadCoreAssembly(const std::filesystem::path& path);
		static void LoadAppAssembly(const std::filesystem::path& path);
		static void PrintCoreAssemblyTypes();
		static void PrintAppAssemblyTypes();
	private:
		static void initMono();
		static void destroyMono();
		static ScriptData* s_Data;
	};
}

#endif