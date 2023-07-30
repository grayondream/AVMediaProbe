#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"
#include "GlobalConfig.h"
#include <filesystem>
#include "Log.h"

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

static std::string time2string(const int64_t t, const AVRational timebase) {
	LOGI("time is {} time base is {}, {}", t, timebase.den, timebase.num);
	std::string ret{}; 
	int64_t n = t * av_q2d(timebase);
	static const char *szs[] = { "second", "minute", "hour", "day"};
	int i = 0;
	while (n && i < 2) {
		ret = std::to_string(n % 60) + TRANS_FETCH(szs[i % 4]) + ret;
		n /= 60;
		i++;
	}

	if (n) {
		ret = std::to_string(n % 24) + TRANS_FETCH(szs[i % 4]) + ret;
		i++;
		n /= 24;
	}

	if (n) {
		ret = std::to_string(n) + TRANS_FETCH(szs[i % 4]) + ret;
	}

	return ret.empty() ? "0" + TRANS_FETCH(szs[0]) : ret;
}

static std::string size2String(const int64_t sz) {
	std::string ret{};
	auto n = sz;
	static const char *szs[] = {"byte ", "kB ", "mB ", "gB ", "pB "};
	int i = 0;
	while (n && i < 4){
		ret = std::to_string(n % 1024) + szs[i % 5] + ret;
		n /= 1024;
		i++;
	}

	if (n) {
		ret = std::to_string(n) + szs[i % 5] + ret;
	}

	return ret.empty() ? "0byte" : ret;
}

json::value FFmpegHelper::info() {
	int er = 0;
	if (!_fmtCtx) {
		do {
			AVDictionary *op = nullptr;
			av_dict_set_int(&op, "probesize", INT64_MAX, 0);
			er = avformat_open_input(&_fmtCtx, _file.data(), nullptr, &op);
			if (er < 0) break;
			er = avformat_find_stream_info(_fmtCtx, nullptr);
			av_dict_free(&op);
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
				{ kStreamDuration, time2string(pstream->duration, pstream->time_base)},
				{ kStreamStartTime, time2string(pstream->start_time, pstream->time_base)},
				{ kStreamBitRate, size2String(pstream->codecpar->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
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

			j[(TRANS_FETCH(kStream) + " " + std::to_string(i)).c_str()] = streamJson;
		}
	}

	{
		json::value mediaJson = json::object{
		{ kFileName, _file.c_str()},
		{ kDuration, time2string(_fmtCtx->duration, AVRational{ 1, AV_TIME_BASE })},
		{ kStartTime, time2string(_fmtCtx->start_time, AVRational{ 1, AV_TIME_BASE })},
		{ kFormat, _fmtCtx->iformat->name},
		{ kMediaFormat, _fmtCtx->iformat->long_name},
		{ kBitRate, size2String(_fmtCtx->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kFormatScore, _fmtCtx->probe_score},
		{ kFilSize, size2String(std::filesystem::file_size(_file.c_str())) }
		};

		j[TRANS_FETCH(kMedia)] = mediaJson;
	}

	return j;
}