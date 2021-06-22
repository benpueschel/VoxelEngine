#include <pch.h>

#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Voxel {

	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] [%n:%l] [pid %P:tid %t] %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("core");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("client");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_CoreLogger->info("Logger initialized");
	}

}