#pragma once

#ifndef HZN_LOGGING_H
#define HZN_LOGGING_H

#include "HorizonEngine/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/fmt.h"
#include "fmt/ostream.h"

#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	class HZN_API Logging
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetAppLogger();
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;
	};

}

#define HZN_CORE_TRACE(...)      ::Hzn::Logging::GetCoreLogger()->trace(__VA_ARGS__)
#define HZN_CORE_INFO(...)       ::Hzn::Logging::GetCoreLogger()->info(__VA_ARGS__)
#define HZN_CORE_WARN(...)       ::Hzn::Logging::GetCoreLogger()->warn(__VA_ARGS__)
#define HZN_CORE_ERROR(...)      ::Hzn::Logging::GetCoreLogger()->error(__VA_ARGS__)
#define HZN_CORE_DEBUG(...)      ::Hzn::Logging::GetCoreLogger()->debug(__VA_ARGS__)
#define HZN_CORE_CRITICAL(...)   ::Hzn::Logging::GetCoreLogger()->critical(__VA_ARGS__)

//App log macros
#define HZN_TRACE(...)      ::Hzn::Logging::GetAppLogger()->trace(__VA_ARGS__)
#define HZN_WARN(...)       ::Hzn::Logging::GetAppLogger()->warn(__VA_ARGS__)
#define HZN_INFO(...)       ::Hzn::Logging::GetAppLogger()->info(__VA_ARGS__)
#define HZN_ERROR(...)      ::Hzn::Logging::GetAppLogger()->error(__VA_ARGS__)
#define HZN_DEBUG(...)      ::Hzn::Logging::GetAppLogger()->debug(__VA_ARGS__)
#define HZN_CRITICAL(...)   ::Hzn::Logging::GetAppLogger()->critical(__VA_ARGS__)

#endif // !HZN_LOGGING_H