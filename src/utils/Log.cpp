#include "Log.h"
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include <vector>

using std::vector;
using std::string;
using std::shared_ptr;

#define LOG_NAME "Logger"

static spdlog::level::level_enum LogLevel2SpdlogLevel(const LogLevel level) {
	switch (level){
	case LogLevel::Log_None: //pass through
	case LogLevel::Log_Trace:
		return spdlog::level::level_enum::trace;
	case LogLevel::Log_Debug:
		return spdlog::level::level_enum::debug;
	case LogLevel::Log_Info:
		return spdlog::level::level_enum::info;
	case LogLevel::Log_Warning:
		return spdlog::level::level_enum::warn;
	case LogLevel::Log_Error:
		return spdlog::level::level_enum::err;
	case LogLevel::Log_Off:
		return spdlog::level::level_enum::off;
	default:
		assert(0);
	}
	
	return spdlog::level::level_enum::info;
}


std::shared_ptr<spdlog::logger> Log::_logger = nullptr;
LogLevel Log::_logLevel = LogLevel::Log_None;
string Log::_logFile;

void Log::setFile(const std::string &file) {
	_logFile = file;
	constexpr auto level = spdlog::level::level_enum::debug;
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	consoleSink->set_level(level);

	auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file.c_str());
	fileSink->set_level(level);

	vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
	_logger = std::make_shared<spdlog::logger>(LOG_NAME, sinks.begin(), sinks.end());
}

void Log::setLevel(const LogLevel level) {
	auto slevel = LogLevel2SpdlogLevel(level);
	_logger->sinks()[1]->set_level(slevel);
}
