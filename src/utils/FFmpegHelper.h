#pragma once
#include <string>

#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
}
#pragma warning(pop)
#include <configor/json.hpp>

using configor::json;


class FFmpegHelper{
public:
	FFmpegHelper(const std::string &file) { _file = file; }
	~FFmpegHelper();
	json::value info();

public:
    std::string _file;
	AVFormatContext *_fmtCtx{};
};