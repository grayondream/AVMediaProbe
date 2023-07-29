#include "Contrller.h"
#include "FFmpegHelper.h"

Controller::Controller() {
}

ErrorCode Controller::insert(const std::string &key) {
	_infos[key] = std::make_shared<FFmpegHelper>(key);
	_infos[key]->findInfo();
	return ErrorCode::None;
}

bool Controller::contain(const std::string &key) {
	return _infos.find(key) != _infos.end();
}