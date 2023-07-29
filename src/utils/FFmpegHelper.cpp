#include "FFmpegHelper.h"

FFmpegHelper::~FFmpegHelper() {
	if (_fmtCtx) {
		avformat_close_input(&_fmtCtx);
		_fmtCtx = nullptr;
	}
}


json::value FFmpegHelper::info() {
	int er = 0;
	if (!_fmtCtx) {
		do {
			er = avformat_open_input(&_fmtCtx, _file.c_str(), nullptr, nullptr);
			if (er < 0) break;
			er = avformat_find_stream_info(_fmtCtx, nullptr);
		} while (false);

		return er;
	}

	
}