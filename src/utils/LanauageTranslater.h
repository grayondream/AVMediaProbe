#pragma once
#include <string>
#include <QString>
#include <QFile>
#include <configor/json.hpp>
#include "def.h"

using configor::json;
class LangTranslate{
public:
	LangTranslate() {}

    std::string get(const std::string &key);
	ErrorCode update(const std::string file);

private:
private:
    std::string _file;
	json::value _json;
};


class LangTranslater {
	LangTranslater() {}
public:
	static LangTranslater &instance();
	std::string get(const std::string &key);
	ErrorCode update(const std::string file);
private:
	static LangTranslate _trans;
};