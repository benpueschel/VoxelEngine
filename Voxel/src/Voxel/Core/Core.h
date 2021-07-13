#pragma once

#include <memory>
#include "PlatformDetection.h"
#include "Voxel/Debug/Assert.h"

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// DLL support
#ifdef VOXEL_PLATFORM_WINDOWS
	#if VOXEL_DYNAMIC_LINK
		#ifdef VOXEL_BUILD_DLL
			#define VOXEL_API __declspec(dllexport)
		#else
			#define VOXEL_API __declspec(dllimport)
		#endif
	#else
		#define VOXEL_API
	#endif
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