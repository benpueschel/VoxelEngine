#include <pch.h>

#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Voxel {

	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	static const std::string pattern = "%^[%T] [%n:%l] [%s:%#] %v%$";

	static std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;

	void Logger::Init()
	{
		AddSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		RecreateLoggers();
		LOG_CORE_INFO("Logger initialized");
	}

	void Logger::AddSink(const std::shared_ptr<spdlog::sinks::sink>& sink)
	{
		sinks.push_back(sink);
	}

	void Logger::RecreateLoggers()
	{
		spdlog::set_pattern(pattern);

		s_CoreLogger = std::make_shared<spdlog::logger>("CORE", sinks.begin(), sinks.end());
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("CLIENT", sinks.begin(), sinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}