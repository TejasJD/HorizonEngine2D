#include "pch.h"
#include "FunctionRegistry.h"

namespace Hzn
{
	class GameObject;

	std::unordered_map<std::string, std::function<void(GameObject&)>> AddComponentFnTable;
	std::unordered_map<std::string, std::function<bool(const GameObject&)>> HasComponentFnTable;
}