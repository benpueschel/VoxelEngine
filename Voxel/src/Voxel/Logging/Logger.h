#pragma once

#include "Voxel/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Voxel {

	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define LOG_CORE_INFO(...)		::Voxel::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_DEBUG(...)		::Voxel::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define LOG_CORE_WARN(...)		::Voxel::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)		::Voxel::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...)		::Voxel::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define LOG_INFO(...)			::Voxel::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)			::Voxel::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define LOG_WARN(...)			::Voxel::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)			::Voxel::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)			::Voxel::Logger::GetClientLogger()->critical(__VA_ARGS__)