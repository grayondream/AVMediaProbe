#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <configor/json.hpp>

using configor::json;

enum class ErrorCode : int32_t {
    None,
    FAILED = -1
};

using ControllerKey = std::string;
using ControllerValue = std::string;
class FFmpegHelper;
class Controller{
public:
	Controller();

public:
    ErrorCode insert(const std::string &file);
	bool contain(const std::string &key);
	json::value info(const std::string &file);

private:
	std::unordered_map < ControllerKey, std::shared_ptr<FFmpegHelper>> _infos;
};