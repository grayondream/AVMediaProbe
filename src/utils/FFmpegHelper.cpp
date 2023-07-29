#include "FFmpegHelper.h"

int FFmpegHelper::findInfo() {
	int er = 0;
	if (!_fmtCtx) {
		er = avformat_open_input(&_fmtCtx, _file.c_str(), nullptr, nullptr);
	}

	return er;
}