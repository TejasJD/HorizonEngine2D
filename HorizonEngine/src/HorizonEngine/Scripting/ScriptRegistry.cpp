#include "pch.h"
#include "ScriptRegistry.h"

#define HZN_ADD_INTERNAL_CALL( name ) mono_add_internal_call("Hzn.InternalCalls::"#name, name)


namespace Hzn
{
	//!Communicating from c++ to c#
	static void Greet()
	{
		std::cout << "Hello called from C++" << std::endl;
	}

	//! calls greet a function to test if c# registers functions
	void ScriptRegistry::registerFunctions()
	{
		HZN_ADD_INTERNAL_CALL(Greet);
	}
}
