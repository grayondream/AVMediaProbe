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

template<typename ... Args>
static std::string str_format(const std::string &format, Args ... args){
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return std::string("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size_buf - 1);
}

static std::string time2string(const int64_t t, const AVRational timebase) {
	LOGI("time is {} time base is {}, {}", t, timebase.den, timebase.num);
	std::string ret{}; 
	double ms = t * av_q2d(timebase);
	if (ms - int64_t(ms) > 0.000000001) {
		ret = std::to_string((ms - int64_t(ms)) * 1000) + TRANS_FETCH(kMSeconds) + ret;
	}
	int64_t n = ms;
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

json::value parseMedia(const AVFormatContext *fmt) {
	return json::object{
		{ kFileName, fmt->url},
		{ kDuration, time2string(fmt->duration, AVRational{ 1, AV_TIME_BASE })},
		{ kStartTime, time2string(fmt->start_time, AVRational{ 1, AV_TIME_BASE })},
		{ kFormat, fmt->iformat->name},
		{ kMediaFormat, fmt->iformat->long_name},
		{ kBitRate, size2String(fmt->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kFormatScore, fmt->probe_score},
		{ kFilSize, size2String(std::filesystem::file_size(fmt->url)) }
	};
}

void parseCommonStream(json::value &streamJson, const AVFormatContext *fmt, const AVStream *pstream) {
	AVCodecParameters *pp = pstream->codecpar;
	streamJson = json::object{
		{ kStreamIndex, pstream->index},
		{ kStreamType, streamType2String(pstream->codecpar->codec_type).c_str()},
		{ kStreamDuration, time2string(pstream->duration, pstream->time_base)},
		{ kStreamStartTime, time2string(pstream->start_time, pstream->time_base)},
		{ kStreamBitRate, size2String(pstream->codecpar->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kStreamCodec, avcodec_get_name(pstream->codecpar->codec_id) },
	};
}

int findNumberBit(int n) {
	int i{};
	while (n) {
		i++;
		n /= 10;
	}

	return i + 1;
}

json::value pkts2json(std::vector<AVPacket*>& pkts) {
	json::value frames{};

	int i = 0;
	auto f = "%0" + std::to_string(findNumberBit(pkts.size())) + "d";
	for (auto p : pkts) {
		std::string str{};

		str += "packetsize=" + std::to_string(p->size);
		str += " dts=" + std::to_string(p->dts);
		str += " pts=" + std::to_string(p->pts);
		str += " duration=" + std::to_string(p->duration);
		str += " position=" + std::to_string(p->pos);
		str += " keyframe=" + std::to_string(!!(p->flags & AV_PKT_FLAG_KEY));
		frames[TRANS_FETCH(kStreamFrame) + str_format(f.c_str(), i++)] = str;
#if 1
		if (i > 10) break;
#endif
	}
	return frames;
}

void parseVideoStream(json::value &streamJson, const AVFormatContext *fmt, const AVStream *ps, std::vector<AVPacket*> pkts) {
	AVCodecParameters *pp = ps->codecpar;
	streamJson[kVideoWidth] = pp->width;
	streamJson[kVideoHeight] = pp->height;
	streamJson[kVideoColorPri] = (int)pp->color_primaries;
	streamJson[kVideoColorRange] = (int)pp->color_range;
	streamJson[kVideoColorSpace] = (int)pp->color_space;
	streamJson[kVideoColorTrc] = (int)pp->color_trc;
	streamJson[kStreamFramesNumber] = pkts.size();
	streamJson[kStreamFrames] = pkts2json(pkts);
}

void parseAudioStream(json::value &streamJson, const AVFormatContext *fmt, const AVStream *ps, std::vector<AVPacket*> pkts) {
	AVCodecParameters *pp = ps->codecpar;
	streamJson[kAudioSampltRate] = std::to_string(pp->sample_rate * 1.0 / 1000) + TRANS_FETCH(kHz); 
	streamJson[kAudioChannel] = (int)pp->channels;
	streamJson[kAudioChannelLayout] = (int)pp->channel_layout;
	streamJson[kStreamFramesNumber] = pkts.size();
	streamJson[kStreamFrames] = pkts2json(pkts);
}

std::vector<std::vector<AVPacket*>> readAllPacket(const AVFormatContext *fmt) {
	std::vector<std::vector<AVPacket*>> pkts{ AVMEDIA_TYPE_NB, std::vector<AVPacket*>{} };
	
	while (true) {
		AVPacket* pkt = av_packet_alloc();
		if(av_read_frame(const_cast<AVFormatContext*>(fmt), pkt) < 0) break;
		pkts[fmt->streams[pkt->stream_index]->codecpar->codec_type].emplace_back(pkt);
	}

	return pkts;
}

void releaseAllPacket(std::vector<std::vector<AVPacket*>> &pkt) {
	for (auto i : pkt) {
		for (std::size_t j = 0; j < i.size();j ++) {
			av_packet_free(&i[j]);
		}
	}
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
	auto pkts = readAllPacket(_fmtCtx);
	json::value j;
	for (int i = 0; i < static_cast<int>(_fmtCtx->nb_streams); i++) {
		AVStream *pstream = _fmtCtx->streams[i];
		json::value streamJson{};
		parseCommonStream(streamJson, _fmtCtx, pstream);
		if (pstream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			parseVideoStream(streamJson, _fmtCtx, pstream, pkts[AVMEDIA_TYPE_VIDEO]);
		}else if(pstream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			parseAudioStream(streamJson, _fmtCtx, pstream, pkts[AVMEDIA_TYPE_AUDIO]);
		}

		j[(TRANS_FETCH(kStream) + " " + std::to_string(i)).c_str()] = streamJson;
	}

	j[TRANS_FETCH(kMedia)] = parseMedia(_fmtCtx);
	releaseAllPacket(pkts);
	return j;
}