#include "LanauageTranslater.h"
#include "Log.h"
#include <cstdio>
#include "def.h"
#include <fstream>

ErrorCode LangTranslate::update(const std::string file) {
	_file = file;

	std::fstream fp{ file, std::fstream::in};
	if (!fp.is_open()) {
		return ErrorCode::IO_FAILED;
	}
	_json = json::parse(fp);
	LOGI("{}", json::dump(_json).c_str());
	fp.close();
	return ErrorCode::None;
}

std::string LangTranslate::get(const std::string &key) {
	if (_json.find(key) == _json.end()) return key;
	return _json[key];
}


LangTranslate LangTranslater::_trans{};

LangTranslater& LangTranslater::instance() {
	static LangTranslater ins;
	return ins;
}

std::string LangTranslater::get(const std::string &key) {
	return _trans.get(key);
}

ErrorCode LangTranslater::update(const std::string file) {
	return _trans.update(file);
}