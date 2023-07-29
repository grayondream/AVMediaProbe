﻿#include "Contrller.h"
#include "FFmpegHelper.h"

Controller::Controller() {
}

ErrorCode Controller::insert(const std::string &key) {
	_infos[key] = std::make_shared<FFmpegHelper>(key);
	return ErrorCode::None;
}

json::value Controller::info(const std::string &file) {
	return _infos[file]->info();
}


bool Controller::contain(const std::string &key) {
	return _infos.find(key) != _infos.end();
}