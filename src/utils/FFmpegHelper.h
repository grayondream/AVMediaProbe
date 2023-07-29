#pragma once
#include <string>

#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
}
#pragma warning(pop)


class FFmpegHelper{
public:
	FFmpegHelper(const std::string &file) { _file = file; }

	int findInfo();

public:
    std::string _file;
	AVFormatContext *_fmtCtx{};
};