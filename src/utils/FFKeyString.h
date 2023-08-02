#pragma once


#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
}
#pragma warning(pop)
#include <configor/json.hpp>

std::string to_string(const AVColorPrimaries r);

std::string to_string(const AVRational a, bool b = false);

std::string streamType2String(const int type);

std::string time2string(const int64_t t, const AVRational timebase);

std::string size2String(const int64_t sz);

template<typename ... Args>
static std::string str_format(const std::string &format, Args ... args) {
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return std::string("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size_buf - 1);
}