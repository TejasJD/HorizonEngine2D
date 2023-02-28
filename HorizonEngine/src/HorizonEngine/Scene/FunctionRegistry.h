#pragma once

#ifndef HZN_FUNCTION_REGISTRY_H
#define HZN_FUNCTION_REGISTRY_H

#include "HorizonEngine/Scene/Component.h"
#include "HorizonEngine/Scene/GameObject.h"

namespace Hzn
{
	extern std::unordered_map<std::string, std::function<void(GameObject&)>> AddComponentFnTable;
	extern std::unordered_map<std::string, std::function<bool(const GameObject&)>> HasComponentFnTable;

	extern std::unordered_map<uint32_t, std::function<void(uint32_t)>> g_CollisionEnterFunctionMap;
	extern std::unordered_map<uint32_t, std::function<void(uint32_t)>> g_CollisionExitFunctionMap;
	extern std::unordered_map<uint32_t, std::function<void(uint32_t)>> g_TriggerEnterFunctionMap;
	extern std::unordered_map<uint32_t, std::function<void(uint32_t)>> g_TriggerExitFunctionMap;

	template<typename... Component>
	void RegisterComponentFunctions(ComponentGroup<Component...>)
	{
		([&]
			{
				AddComponentFnTable[typeid(Component).name()] = &GameObject::addComponent<Component>;
				HasComponentFnTable[typeid(Component).name()] = &GameObject::hasComponent<Component>;
			}(), ...);
	}
};

#endif