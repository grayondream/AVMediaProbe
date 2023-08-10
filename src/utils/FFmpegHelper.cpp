#include "FFmpegHelper.h"
#include "FFmpegSerialKey.h"
#include "GlobalConfig.h"
#include <filesystem>
#include "Log.h"
#include "FFKeyString.h"
#include "fmath.h"

#pragma warning(push, 0)
extern "C" {
#   include <libavformat/avformat.h>
#	include <libavutil/avutil.h>
#	include <libavutil/display.h>
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

json::value string2json(const std::string &str) {
	try {
		return json::parse(str);
	}
	catch (std::exception &) {
		return str;
	}
}

json::value parseMetaData(const AVDictionary *metadata) {
	if (!metadata) return {};
	json::value j;
	AVDictionaryEntry *tag = NULL;
	while ((tag = av_dict_get(metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
		LOGI("{} : {}");
		std::string str(tag->value);
		if (str.find("{") != std::string::npos && str.find("}") != std::string::npos) {
			j[TRANS_FETCH(tag->key)] = string2json(tag->value);
		} else {
			j[TRANS_FETCH(tag->key)] = tag->value;
		}
		
	}

	return j;
}

json::value parseMedia(const std::shared_ptr<FileContext> pc) {
	auto fmt = pc->_fmtCtx;
	pc->_filesize = std::filesystem::file_size(fmt->url);
	json::value j = json::object{
		{ kFileName, fmt->url},
		{ kDuration, time2string(fmt->duration, AVRational{ 1, AV_TIME_BASE })},
		{ kStartTime, time2string(fmt->start_time, AVRational{ 1, AV_TIME_BASE })},
		{ kFormat, fmt->iformat->name},
		{ kMediaFormat, fmt->iformat->long_name},
		{ kBitRate, size2String(fmt->bit_rate) + "/" + TRANS_FETCH(kSeconds)},
		{ kFormatScore, fmt->probe_score},
		{ kFilSize, size2String(pc->_filesize) }
	};

	if (pc->_fmtCtx->metadata) {
		j[kMetaData] = parseMetaData(pc->_fmtCtx->metadata);
	}
	
	return j;
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

	if (pc->_streams[index]._pstream->metadata) {
		j[kMetaData] = parseMetaData(pc->_streams[index]._pstream->metadata);
	}
	
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

static json::value parseTransMatrix(const int32_t* m) {
	json::value j{};
	std::string ps[] = { kVideoTransMatrixabu, kVideoTransMatrixcdv, kVideoTransMatrixxyw };
	for (int i = 0; i < 3; i++) {
		std::string ret{};
		ret += "[";
		for (int j = 0; j < 3; j++) {
			ret += "  ";
			if (j == 2) {
				//2.30
				ret += std::to_string(int32todouble(*(m + i * 3 + j), 2, 30));
			}
			else {
				//16.16
				ret += std::to_string(int32todouble(*(m + i * 3 + j), 16, 16)) + ",";
			}

			LOGI("the value is {}", *(m + i * 3 + j));
		}

		j[ps[i]] = ret + "]";
	}
	return j;
}

int getsign(const int32_t v) {
	return v == 0 ? 0 : v / std::abs(v);
}

void printMatrix(int32_t *p, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d ", p[i * row + j]);
		}

		printf("\n");
	}
}

void parseVideoTransformer(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto st = pc->_streams[index]._pstream;
	uint8_t* pd = (uint8_t*)av_stream_get_side_data(st.get(), AV_PKT_DATA_DISPLAYMATRIX, NULL);
	double theta = 0;
	if (pd) {
		int32_t* displaymatrix = (int32_t*)pd;
		printMatrix(displaymatrix, 3, 3);
		j[kVideoTransMatrix] = parseTransMatrix(displaymatrix);
		bool isflipx = displaymatrix[6] < 0;
		j[kVideoFlipX] = isflipx ? TRANS_FETCH(kYes) : TRANS_FETCH(kNo);
		av_display_matrix_flip(displaymatrix, displaymatrix[6] < 0, 0);
		printMatrix(displaymatrix, 3, 3);

		bool isflipy = (getsign(displaymatrix[0]) * getsign(displaymatrix[4]) == -1) || (getsign(displaymatrix[1]) * getsign(displaymatrix[3]) == 1);
		j[kVideoFlipY] = isflipy ? TRANS_FETCH(kYes) : TRANS_FETCH(kNo);
		
		av_display_matrix_flip(displaymatrix, false, isflipy);
		printMatrix(displaymatrix, 3, 3);
		theta = -av_display_rotation_get(displaymatrix);
	}

	j[kVideoRatation] = int(theta);
	
}

static bool isinvalid(const AVRational r) {
	return r.den == 0 || r.num == 0;
}

void parseVideoAspectRatio(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto ps = pc->_streams[index]._pstream;
	auto& pkts = pc->_streams[index]._pkts;
	AVCodecParameters *pp = ps->codecpar;
	AVRational sar = isinvalid(pp->sample_aspect_ratio) ? AVRational{ 1,1 } : pp->sample_aspect_ratio;
	auto gcdint = gcd<int>(pp->width, pp->height);
	AVRational par = AVRational{ pp->width / gcdint, pp->height / gcdint };
	AVRational dar = av_mul_q(par, sar);
	j[kVideoPar] = to_string(par, false, ':');
	j[kVideoSar] = to_string(sar, false, ':');
	j[kVideoDar] = to_string(dar, false, ':');
}

void parseVideoStream(json::value &j, const std::shared_ptr<FileContext> pc, int index) {
	auto ps = pc->_streams[index]._pstream;
	auto& pkts = pc->_streams[index]._pkts;
	AVCodecParameters *pp = ps->codecpar;
	j[kVideoWidth] = std::to_string(pp->width) + TRANS_FETCH(kPixel);
	j[kVideoHeight] = std::to_string(pp->height) + TRANS_FETCH(kPixel);
	j[kVideoColorPri] = to_string(pp->color_primaries);
	j[kVideoColorRange] = to_string(pp->color_range);
	j[kVideoColorSpace] = to_string(pp->color_space);
	j[kVideoColorTrc] = to_string(pp->color_trc);
	j[kStreamFramesNumber] = pkts.size();
	j[kStreamFrames] = pkts2json(pkts);
	j[kVideoDataDensity] = pp->bit_rate / (pp->width * pp->height * av_q2d(ps->avg_frame_rate));
	parseVideoFormat(j, static_cast<AVPixelFormat>(pp->format));
	parseVideoTransformer(j, pc, index);
	parseVideoAspectRatio(j, pc, index);
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
	return 0;
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