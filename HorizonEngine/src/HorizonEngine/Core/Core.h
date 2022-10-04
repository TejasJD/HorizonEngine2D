#pragma once

#ifndef HZN_CORE_H
#define HZN_CORE_H

#ifdef HZN_PLATFORM_WINDOWS // if platform is windows
#ifdef HZN_BUILD // import and export macro for DLL.
#define HZN_API __declspec(dllexport)
#else
#define HZN_API __declspec(dllimport)
#endif
#else
#error platform windows needed!
#endif

#ifdef HZN_ASSERTS_ON
	#define HZN_ASSERT(x, ...) { if(!(x)) { HZN_ERROR("{0}", __VA_ARGS__); __debugbreak(); } }
	#define HZN_CORE_ASSERT(x, ...) { if(!(x)) { HZN_CORE_ERROR("{0}", __VA_ARGS__); __debugbreak(); }
#else
	#define HZN_ASSERT(x, ...)
	#define HZN_CORE_ASSERT(x, ...)
#endif

#endif // !HZN_CORE_H