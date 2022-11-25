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

		static void LoadAssembly(const std::filesystem::path& path);
	private:
		static void initMono();
		static void destroyMono();
		static ScriptData* s_Data;
	};
}

#endif