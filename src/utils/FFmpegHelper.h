#pragma once
#include <string>
#include <configor/json.hpp>

using configor::json;

struct FileContext;
class FFmpegHelper{
public:
	FFmpegHelper(const std::string &file) { _file = file; }
	~FFmpegHelper();
	json::value info();

	int init(const std::string& file);

public:
    std::string _file;
	std::shared_ptr<FileContext> _filectx;
};