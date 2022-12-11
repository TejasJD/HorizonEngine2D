#include "pch.h"
#include "FunctionRegistry.h"

namespace Hzn
{
	class GameObject;

	std::unordered_map<std::string, std::function<void(GameObject&)>> AddComponentFnTable;
	std::unordered_map<std::string, std::function<bool(const GameObject&)>> HasComponentFnTable;
	std::map<uint32_t, std::function<void(uint32_t)>> g_CollisionEnterFunctionMap;
	std::map<uint32_t, std::function<void(uint32_t)>> g_CollisionExitFunctionMap;
	std::map<uint32_t, std::function<void(uint32_t)>> g_TriggerEnterFunctionMap;
	std::map<uint32_t, std::function<void(uint32_t)>> g_TriggerExitFunctionMap;
}