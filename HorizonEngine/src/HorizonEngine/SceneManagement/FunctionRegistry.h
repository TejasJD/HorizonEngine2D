#pragma once

#ifndef HZN_FUNCTION_REGISTRY_H
#define HZN_FUNCTION_REGISTRY_H

#include "HorizonEngine/Components/Component.h"
#include "GameObject.h"

namespace Hzn
{
	extern std::unordered_map<std::string, std::function<void(GameObject&)>> AddComponentFnTable;
	extern std::unordered_map<std::string, std::function<bool(const GameObject&)>> HasComponentFnTable;

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