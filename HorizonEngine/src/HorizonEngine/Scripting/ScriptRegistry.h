#pragma once

#ifndef HZN_SCRIPT_REGISTRY_H
#define HZN_SCRIPT_REGISTRY_H

#include <mono/jit/jit.h>

namespace Hzn
{
	class ScriptRegistry
	{
	public:
		//! calls greet a function to test if c# registers functions
		static void registerFunctions();
		static void registerComponents();
	};
}

#endif