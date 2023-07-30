#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"

FFmpegHelper::~FFmpegHelper() {
	if (_fmtCtx) {
		avformat_close_input(&_fmtCtx);
		_fmtCtx = nullptr;
	}
}

static std::string streamType2String(const int type) {
	switch (type) {
	case AVMEDIA_TYPE_AUDIO:
		return "audio";
	case AVMEDIA_TYPE_SUBTITLE:
		return "subtitle";
	case AVMEDIA_TYPE_VIDEO:
		return "video";
	default:
		return "unknown";
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
		for (int i = 0; i < static_cast<int>(_fmtCtx->nb_streams); i++) {
			AVStream *pstream = _fmtCtx->streams[i];
			AVCodecParameters *pp = pstream->codecpar;
			json::value streamJson = json::object{
				{ kStreamIndex, pstream->index},
				{ kStreamType, streamType2String(pstream->codecpar->codec_type).c_str()},
				{ kDuration, pstream->duration},
				{ kStartTime, pstream->start_time},
				{ kBitRate, pstream->codecpar->bit_rate},
				{ kStreamCodec, avcodec_get_name(pstream->codecpar->codec_id) },
				
			};

			if (pstream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				streamJson[kVideoWidth] = pp->width;
				streamJson[kVideoHeight] = pp->height;
				streamJson[kVideoColorPri] = (int)pp->color_primaries;
				streamJson[kVideoColorRange] = (int)pp->color_range;
				streamJson[kVideoColorSpace] = (int)pp->color_space;
				streamJson[kVideoColorTrc] = (int)pp->color_trc;
			}

			j[std::to_string(i).c_str()] = streamJson;
		}
	}

	{
		json::value mediaJson = json::object{
		{ kFileName, _file.c_str()},
		{ kDuration, _fmtCtx->duration},
		{ kStartTime, _fmtCtx->start_time},
		{ kFormat, _fmtCtx->iformat->name},
		{ kMediaFormat, _fmtCtx->iformat->long_name},
		{ kBitRate, _fmtCtx->bit_rate},
		{ kFormatScore, _fmtCtx->probe_score},
		};

		j[kMedia] = mediaJson;
	}
	return j;
}