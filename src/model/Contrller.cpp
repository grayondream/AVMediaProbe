#include "Contrller.h"
#include "FFmpegHelper.h"

Controller::Controller() {
}

ErrorCode Controller::insert(const std::string &key) {
	_infos[key] = std::make_shared<FFmpegHelper>(key);
	return ErrorCode::None;
}

json::value Controller::info(const std::string &file) {
	if (_infosJson.find(file) == _infosJson.end()) {
		_infosJson[file] = _infos[file]->info();
	}

	return _infosJson[file];
}


bool Controller::contain(const std::string &key) {
	return _infos.find(key) != _infos.end();
}