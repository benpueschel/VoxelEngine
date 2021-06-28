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

#define LOG_CORE_TRACE(...)		SPDLOG_LOGGER_TRACE(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)
#define LOG_CORE_INFO(...)		SPDLOG_LOGGER_INFO(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)
#define LOG_CORE_DEBUG(...)		SPDLOG_LOGGER_DEBUG(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)
#define LOG_CORE_WARN(...)		SPDLOG_LOGGER_WARN(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)
#define LOG_CORE_ERROR(...)		SPDLOG_LOGGER_ERROR(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)
#define LOG_CORE_FATAL(...)		SPDLOG_LOGGER_CRITICAL(Voxel::Logger::GetCoreLogger(), __VA_ARGS__)

#define LOG_TRACE(...)			SPDLOG_LOGGER_TRACE(Voxel::Logger::GetClientLogger(), __VA_ARGS__)
#define LOG_INFO(...)			SPDLOG_LOGGER_INFO(Voxel::Logger::GetClientLogger(), __VA_ARGS__)
#define LOG_DEBUG(...)			SPDLOG_LOGGER_DEBUG(Voxel::Logger::GetClientLogger(), __VA_ARGS__)
#define LOG_WARN(...)			SPDLOG_LOGGER_WARN(Voxel::Logger::GetClientLogger(), __VA_ARGS__)
#define LOG_ERROR(...)			SPDLOG_LOGGER_ERROR(Voxel::Logger::GetClientLogger(), __VA_ARGS__)
#define LOG_FATAL(...)			SPDLOG_LOGGER_CRITICAL(Voxel::Logger::GetClientLogger(), __VA_ARGS__)