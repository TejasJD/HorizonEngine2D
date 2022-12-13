#include "pch.h"
#include "Logging.h"


namespace Hzn
{
	//! Access core logger
	std::shared_ptr<spdlog::logger>Logging::s_CoreLogger;
	//! Access App logger
	std::shared_ptr<spdlog::logger>Logging::s_AppLogger;

	//! Return core logger
	std::shared_ptr<spdlog::logger>& Logging::GetCoreLogger()
	{
		// TODO: insert return statement here
		return s_CoreLogger;
	}

	//! Return App Logger
	std::shared_ptr<spdlog::logger>& Logging::GetAppLogger()
	{
		// TODO: insert return statement here
		return s_AppLogger;
	}

	//! Function to initialise the logging class
	void Logging::Init()
	{

		//! Vector to set colour & file sink to log to a file
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Horizon.log", true));

		logSinks[0]->set_pattern("%^[%Y-%m-%d %H:%M:%S] %n: %v%$");
		logSinks[1]->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("Horizon Engine", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_AppLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_AppLogger);
		s_AppLogger->set_level(spdlog::level::trace);
		s_AppLogger->flush_on(spdlog::level::trace);
	}

}