#pragma once

#ifndef HZN_SCRIPT_ENGINE_H
#define HZN_SCRIPT_ENGINE_H

struct _MonoAssembly;
using MonoAssembly = _MonoAssembly;

namespace Hzn
{
	struct ScriptData;

	class ScriptEngine
	{
	public:
		static void init();
		static void destroy();
	private:
		static void initMono();
		static void destroyMono();
		static char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		static void PrintAssemblyTypes(MonoAssembly* assembly);

		static ScriptData* s_Data;
	};
}

#endif