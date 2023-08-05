#pragma once
#include <string>

#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
}
#pragma warning(pop)
#include <configor/json.hpp>

using configor::json;

class FileContext;
class FFmpegHelper{
public:
	FFmpegHelper(const std::string &file) { _file = file; }
	~FFmpegHelper();
	json::value info();

	int opencodec(AVStream *ps);
	int init(const std::string& file);

public:
    std::string _file;
	std::shared_ptr<FileContext> _filectx;
};