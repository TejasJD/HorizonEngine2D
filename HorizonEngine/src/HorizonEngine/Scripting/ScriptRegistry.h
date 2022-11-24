#pragma once

#ifndef HZN_SCRIPT_REGISTRY_H
#define HZN_SCRIPT_REGISTRY_H

#include <mono/jit/jit.h>

namespace Hzn
{
	class ScriptRegistry
	{
	public:
		static void registerFunctions();
	};
}

#endif