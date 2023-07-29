#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"

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
	}

	if (!_fmtCtx) return {};
	json::value j;
	{
		json::value mediaJson = json::object{
		{ kFileName, _file.c_str()},
		{ kDuration, _fmtCtx->duration}
		};

		j[kMedia] = mediaJson;
	}

	{
		for (int i = 0; i < static_cast<int>(_fmtCtx->nb_streams); i++) {
			AVStream *pstream = _fmtCtx->streams[i];
			json::value streamJson = json::object{
				{ kDuration, pstream->duration}
			};

			j[std::to_string(i).c_str()] = streamJson;
		}
	}
	return j;
}