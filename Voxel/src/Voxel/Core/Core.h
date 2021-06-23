#pragma once

#include <memory>
#include "Voxel/Logging/Logger.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef _WIN32
	#ifdef _WIN64
		#define VOXEL_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define VOXEL_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define VOXEL_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#define VOXEL_API __declspec(dllimport)
		#error "Unknown Apple platform!"
	#endif

/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define VOXEL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define VOXEL_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unkown platform!"
#endif

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