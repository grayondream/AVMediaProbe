/*
 * @brief this file is a simple wrapper of spdlog, which is used to log information easily in this project
 */
#pragma once

#include <string>
#include <memory>

#pragma warning(push, 0)
#	include <spdlog/spdlog.h>
#	include <spdlog/cfg/env.h>
#	include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

enum class LogLevel: int32_t {
	Log_None,
	Log_Trace,
	Log_Debug,
	Log_Info,
	Log_Warning,
	Log_Error,
	Log_Off
};

class Log{
public:
	static void setFile(const std::string &file);

	static void setLevel(const LogLevel level);
public:
	template<typename FormatString, typename... Args>
	static void logt(const FormatString &fmt, const Args &...args) {
		_logger->trace(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void logd(const FormatString &fmt, const Args &...args) {
		_logger->debug(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void logi(const FormatString &fmt, const Args &...args) {
		_logger->info(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void logw(const FormatString &fmt, const Args &...args) {
		_logger->warn(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void loge(const FormatString &fmt, const Args &...args) {
		_logger->error(fmt, args...);
	}
private:

private:
	static LogLevel _logLevel;
	static std::string _logFile;

	static std::shared_ptr<spdlog::logger> _logger;
};

#define LOGSETFILE(file) Log::setFile(file)
#define LOGSETLEVEL(level) Log::setLevel(level);

#define LOGT(...) Log::logt(__VA_ARGS__)
#define LOGD(...) Log::logd(__VA_ARGS__)
#define LOGI(...) Log::logi(__VA_ARGS__)
#define LOGW(...) Log::logw(__VA_ARGS__)
#define LOGE(...) Log::loge(__VA_ARGS__)
