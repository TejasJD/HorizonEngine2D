#pragma once

#ifdef HZN_PLATFORM_WINDOWS // if platform is windows
	#ifdef HZN_BUILD // import and export macro for DLL.
		#define HZN_API __declspec(dllexport)
	#else
		#define HZN_API __declspec(dllimport)
	#endif
#else
	#error platform windows needed!
#endif