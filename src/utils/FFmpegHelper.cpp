#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"
#include "GlobalConfig.h"
#include <filesystem>
#include "Log.h"
#include "FFKeyString.h"

FFmpegHelper::~FFmpegHelper() {
	if (_fmtCtx) {
		avformat_close_input(&_fmtCtx);
		_fmtCtx = nullptr;
	}
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
	auto duration = pstream->duration == AV_NOPTS_VALUE ? fmt->duration : pstream->duration;
	auto timbase = pstream->duration == AV_NOPTS_VALUE ? AVRational{ 1, AV_TIME_BASE }: pstream->time_base;
	streamJson = json::object{
		{ kStreamIndex, pstream->index},
		{ kStreamType, streamType2String(pstream->codecpar->codec_type).c_str()},
		{ kStreamDuration, time2string(duration, timbase)},
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

void parseVideoFormat(json::value &j, const AVPixelFormat fmt) {
	std::vector<std::string> vec = to_string(fmt);
	if (!vec[0].empty()) { j[kVideoFmtFmt] = vec[0]; }
	if (!vec[1].empty()) { j[kVideoFmtLine] = vec[1]; }
	if (!vec[3].empty()) { j[kVideoFmtBit] = vec[3]; }
	if (!vec[2].empty()) { j[kVideoFmtLayout] = vec[2]; }
	if (!vec[5].empty()) { j[kVideoFmtEnd] = vec[5]; }
	if (!vec[6].empty()) { j[kVideoFmtBitFormat] = vec[6]; }
}

void parseVideoStream(json::value &streamJson, const AVFormatContext *fmt, const AVStream *ps, std::vector<AVPacket*> pkts) {
	AVCodecParameters *pp = ps->codecpar;
	streamJson[kVideoWidth] = pp->width;
	streamJson[kVideoHeight] = pp->height;
	streamJson[kVideoColorPri] = to_string(pp->color_primaries);
	streamJson[kVideoColorRange] = to_string(pp->color_range);
	streamJson[kVideoColorSpace] = to_string(pp->color_space);
	streamJson[kVideoColorTrc] = to_string(pp->color_trc);
	streamJson[kStreamFramesNumber] = pkts.size();
	streamJson[kStreamFrames] = pkts2json(pkts);
	parseVideoFormat(streamJson, static_cast<AVPixelFormat>(pp->format));
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