#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"
#include "GlobalConfig.h"
#include <filesystem>
#include "Log.h"
#include "FFKeyString.h"
#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
}
#pragma warning(pop)

struct StreamContext {
	std::vector<std::shared_ptr<AVPacket>> _pkts{};
	int64_t _totalSize{};
	std::shared_ptr<AVCodecContext> _pcodecctx{};
	std::shared_ptr<AVCodec> _pcodec{};
	std::shared_ptr<AVStream> _pstream{};
};

struct FileContext{
	std::shared_ptr<AVFormatContext> _fmtCtx{};
	std::vector<StreamContext> _streams{};
	int64_t _filesize{};
};

FFmpegHelper::~FFmpegHelper() {
	_filectx = nullptr;
}

json::value parseMedia(const std::shared_ptr<FileContext> pc) {
	auto fmt = pc->_fmtCtx;
	pc->_filesize = std::filesystem::file_size(fmt->url);
	return json::object{
		{ kFileName, fmt->url},
		{ kDuration, time2string(fmt->duration, AVRational{ 1, AV_TIME_BASE })},
		{ kStartTime, time2string(fmt->start_time, AVRational{ 1, AV_TIME_BASE })},
		{ kFormat, fmt->iformat->name},
		{ kMediaFormat, fmt->iformat->long_name},
		{ kBitRate, size2String(fmt->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kFormatScore, fmt->probe_score},
		{ kFilSize, size2String(pc->_filesize) }
	};
}

void parseFps(json::value &j, const std::shared_ptr<AVStream> ps) {
	if (!ps) return;
	if (ps->avg_frame_rate.den <= 0) return;
	j[kVideoAVGFps] = std::to_string(av_q2d(ps->avg_frame_rate)) + to_string(ps->avg_frame_rate, true) + "FPS";
	bool isvfr = false;
	if (ps->r_frame_rate.den > 0) {
		isvfr = av_cmp_q(ps->r_frame_rate, ps->avg_frame_rate) != 0;
		j[kVideoMAXFps] = std::to_string(av_q2d(ps->r_frame_rate)) + to_string(ps->r_frame_rate, true) + "FPS";
	}
	
	j[kVideoIsVFR] = isvfr ? TRANS_FETCH(kVFR) : TRANS_FETCH(kCFR);
}

void parseCommonStream(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto pstream = pc->_streams[index]._pstream;
	auto pfmt = pc->_fmtCtx;
	AVCodecParameters *pp = pstream->codecpar;
	auto duration = pstream->duration == AV_NOPTS_VALUE ? pfmt->duration : pstream->duration;
	auto timbase = pstream->duration == AV_NOPTS_VALUE ? AVRational{ 1, AV_TIME_BASE }: pstream->time_base;
	j = json::object{
		{ kStreamIndex, pstream->index},
		{ kStreamType, TRANS_FETCH(streamType2String(pstream->codecpar->codec_type))},
		{ kStreamDuration, time2string(duration, timbase)},
		{ kStreamStartTime, time2string(pstream->start_time, pstream->time_base)},
		{ kStreamBitRate, size2String(pstream->codecpar->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kStreamSize, size2String(pc->_streams[index]._totalSize) + double2percent(pc->_streams[index]._totalSize, pc->_filesize)},
		{ kStreamCodec, avcodec_get_name(pstream->codecpar->codec_id) },
	};

	parseFps(j, pstream);
}

int findNumberBit(int n) {
	int i{};
	while (n) {
		i++;
		n /= 10;
	}

	return i + 1;
}

json::value pkts2json(std::vector<std::shared_ptr<AVPacket>>& pkts) {
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

void parseVideoStream(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto ps = pc->_streams[index]._pstream;
	auto& pkts = pc->_streams[index]._pkts;
	AVCodecParameters *pp = ps->codecpar;
	j[kVideoWidth] = pp->width;
	j[kVideoHeight] = pp->height;
	j[kVideoColorPri] = to_string(pp->color_primaries);
	j[kVideoColorRange] = to_string(pp->color_range);
	j[kVideoColorSpace] = to_string(pp->color_space);
	j[kVideoColorTrc] = to_string(pp->color_trc);
	j[kStreamFramesNumber] = pkts.size();
	j[kStreamFrames] = pkts2json(pkts);
	parseVideoFormat(j, static_cast<AVPixelFormat>(pp->format));
}

void parseAudioStream(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto ps = pc->_streams[index]._pstream;
	AVCodecParameters *pp = ps->codecpar;
	auto& pkts = pc->_streams[index]._pkts;
	j[kAudioSampltRate] = std::to_string(pp->sample_rate * 1.0 / 1000) + TRANS_FETCH(kHz);
	j[kAudioChannel] = (int)pp->channels;
	j[kAudioChannelLayout] = (int)pp->channel_layout;
	j[kStreamFramesNumber] = pkts.size();
	j[kStreamFrames] = pkts2json(pkts);

}

void readAllPkacet(const std::shared_ptr<FileContext> fc) {
	 auto fmt = fc->_fmtCtx;
	if (!fmt || !fmt->nb_streams) return;
	auto& sc = fc->_streams;
	while (true) {
		AVPacket* pkt = av_packet_alloc();
		if (av_read_frame(fmt.get(), pkt) < 0) break;
		sc[pkt->stream_index]._pkts.emplace_back(std::shared_ptr<AVPacket>(pkt, [](AVPacket *pkt) { av_packet_unref(pkt); }));
		sc[pkt->stream_index]._totalSize += pkt->size;
	}
	
}

int opencodec(const std::shared_ptr<FileContext> fc) {
	int err{};
	for (int i = 0; i < (int)fc->_fmtCtx->nb_streams; i++) {
		auto ps = fc->_streams[i]._pstream;
		const AVCodec*pc = avcodec_find_decoder(ps->codecpar->codec_id);
		AVCodecContext *pcc = avcodec_alloc_context3(pc);
		fc->_streams[ps->index]._pcodecctx = std::shared_ptr<AVCodecContext>(pcc, [](AVCodecContext *pc) {
			avcodec_free_context(&pc);
			pc = nullptr;
		});
		fc->_streams[ps->index]._pcodec = std::shared_ptr<AVCodec>(const_cast<AVCodec*>(pc), [](AVCodec *pc) {});
		err = avcodec_open2(pcc, pc, nullptr);
	}
	return err;
}

int FFmpegHelper::init(const std::string& file) {
	int er = 0;
	if (!_filectx) {
		_filectx = std::make_shared<FileContext>();
		do {
			AVDictionary *op = nullptr;
			av_dict_set_int(&op, "probesize", INT64_MAX, 0);
			AVFormatContext *pfc{};
			er = avformat_open_input(&pfc, _file.data(), nullptr, &op);
			if (er < 0) break;
			_filectx->_fmtCtx = std::shared_ptr<AVFormatContext>(pfc, [](AVFormatContext *pc) { avformat_close_input(&pc); });
			er = avformat_find_stream_info(pfc, nullptr);
			av_dict_free(&op);
			for (int i = 0; i < (int)pfc->nb_streams; i++) {
				StreamContext st{};
				st._pstream = std::shared_ptr<AVStream>(pfc->streams[i], [](const AVStream *ps) {});
				_filectx->_streams.emplace_back(st);
			}

			er = opencodec(_filectx);
			if (er < 0) break;
			readAllPkacet(_filectx);
		} while (false);
	}

	return er;
}

json::value FFmpegHelper::info() {
	if (!_filectx && init(_file) < 0) {
		return {};
	}

	json::value j;
	j[TRANS_FETCH(kMedia)] = parseMedia(_filectx);
	for (int i = 0; i < static_cast<int>(_filectx->_streams.size()); i++) {
		json::value jj{};
		parseCommonStream(jj, _filectx, i);
		auto pstream = _filectx->_streams[i]._pstream;
		if (pstream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			parseVideoStream(jj, _filectx, i);
		}else if(pstream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			parseAudioStream(jj, _filectx, i);
		}

		j[(TRANS_FETCH(kStream) + " " + std::to_string(i)).c_str()] = jj;
	}

	return j;
}