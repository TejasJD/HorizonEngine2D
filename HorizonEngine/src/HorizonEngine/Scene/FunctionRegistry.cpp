#include "pch.h"

#include "HorizonEngine/Scene/FunctionRegistry.h"

namespace Hzn {
class GameObject;

std::unordered_map<std::string, std::function<void(GameObject&)>>
    AddComponentFnTable;
std::unordered_map<std::string, std::function<bool(const GameObject&)>>
    HasComponentFnTable;
std::unordered_map<uint32_t, std::function<void(uint32_t)>>
    g_CollisionEnterFunctionMap;
std::unordered_map<uint32_t, std::function<void(uint32_t)>>
    g_CollisionExitFunctionMap;
std::unordered_map<uint32_t, std::function<void(uint32_t)>>
    g_TriggerEnterFunctionMap;
std::unordered_map<uint32_t, std::function<void(uint32_t)>>
    g_TriggerExitFunctionMap;
}  // namespace Hzn