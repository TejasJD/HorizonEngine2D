#pragma once

#include "HorizonEngine/APIMacros.h"
#include "pch.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>


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