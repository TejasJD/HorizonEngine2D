#pragma once

#ifndef HZN_CORE_H
#define HZN_CORE_H

#include <memory>

#include "HorizonEngine/Logging/Logging.h"

#define HZN_API

#ifdef HZN_ASSERTS_ON
#define HZN_CORE_ASSERT(x, ...)  if(!(x)) { HZN_CORE_ERROR(__VA_ARGS__); __debugbreak(); } else {}
#define HZN_ASSERT(x, ...) if(!(x)) { HZN_ERROR(__VA_ARGS__); __debugbreak(); } else {}
#else
#define HZN_CORE_ASSERT(x, ...)
#define HZN_ASSERT(x, ...)
#endif

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
Ref<T> MakeRef(Args&& ...args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Scope<T> MakeScope(Args&& ...args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif