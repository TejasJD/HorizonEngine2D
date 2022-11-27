#include "pch.h"
#include "Component.h"
#include "HorizonEngine/SceneManagement/FunctionRegistry.h"

namespace Hzn
{
	void addComponent(GameObject& obj, std::string& componentName)
	{
		if (componentName.find("struct Hzn::") == std::string::npos)
		{
			componentName = std::string("struct Hzn::") + componentName;
		}

		const auto& it = HasComponentFnTable.find(componentName);
		if (it == HasComponentFnTable.end())
		{
			HZN_CORE_ERROR("Component Name Invalid!");
			throw std::runtime_error("Component Name Invalid!");
		}

		if(!it->second(obj))
		{
			AddComponentFnTable[componentName](obj);
		}
	}

	bool hasComponent(const GameObject& obj, std::string& componentName)
	{
		if (componentName.find("struct Hzn::") == std::string::npos)
		{
			componentName = std::string("struct Hzn::") + componentName;
		}
		
		const auto& it = HasComponentFnTable.find(componentName);
		if(it == HasComponentFnTable.end())
		{
			HZN_CORE_ERROR("Component Name Invalid!");
			throw std::runtime_error("Component Name Invalid!");
		}
		return it->second(obj);
	}
}
