#include "pch.h"
#include "ScriptRegistry.h"

#define HZN_ADD_INTERNAL_CALL( name ) mono_add_internal_call("Hzn.InternalCalls::"#name, name)


namespace Hzn
{
	static void Greet()
	{
		std::cout << "Hello called from C++" << std::endl;
	}

	void ScriptRegistry::registerFunctions()
	{
		HZN_ADD_INTERNAL_CALL(Greet);
	}
}
