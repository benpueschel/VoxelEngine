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

		static void AddSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
		static void RecreateLoggers();

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define LOGGER_SRC(logger, level, ...) (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define LOG_CORE_TRACE(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::trace, __VA_ARGS__)
#define LOG_CORE_INFO(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::info, __VA_ARGS__)
#define LOG_CORE_DEBUG(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::debug, __VA_ARGS__)
#define LOG_CORE_WARN(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_CORE_ERROR(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::err, __VA_ARGS__)
#define LOG_CORE_FATAL(...)		LOGGER_SRC(Voxel::Logger::GetCoreLogger(), spdlog::level::critical, __VA_ARGS__)

#define LOG_TRACE(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::trace, __VA_ARGS__)
#define LOG_INFO(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::info, __VA_ARGS__)
#define LOG_DEBUG(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::debug, __VA_ARGS__)
#define LOG_WARN(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(...)			LOGGER_SRC(Voxel::Logger::GetClientLogger(), spdlog::level::critical, __VA_ARGS__)