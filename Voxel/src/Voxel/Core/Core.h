#pragma once

#include <memory>
#include "Voxel/Logging/Logger.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef PLATFORM_WINDOWS

#else
	#error Engine currently only supporting Windows
#endif

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) { if(!x) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CORE_ASSERT(x, ...) { if(!x) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
	#define CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

namespace Voxel {

	template<typename T>
	using Scope = std::unique_ptr<T>; 
	
	template<typename T, typename ... Args>
		constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}