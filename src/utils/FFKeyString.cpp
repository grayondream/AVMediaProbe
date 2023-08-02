#include <string>
#include "FFKeyString.h"
#include "GlobalConfig.h"
#include "Log.h"
#include "FFmpegSerialKey.h"

std::string to_string(const AVRational a, bool b) {
	return b ? "(" + std::to_string(a.num) + "," + std::to_string(a.den) + ")" : std::to_string(a.num) + "," + std::to_string(a.den);
}

std::string to_string(const AVColorPrimaries r) {
	switch (r) {
	case AVCOL_PRI_RESERVED0:
		return "reserved";
	case AVCOL_PRI_BT709:  ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP 177 Annex B
		return "BT.709";
	case AVCOL_PRI_UNSPECIFIED:
		return "unspecified";
	case AVCOL_PRI_RESERVED:
		return "reserved";
	case AVCOL_PRI_BT470M:
		return "BT.470M";
	case AVCOL_PRI_BT470BG:
		return "BT.470BG";
	case AVCOL_PRI_SMPTE170M:
		return "SMPTE.170M";
	case AVCOL_PRI_SMPTE240M:
		return "SMPTE.240M";
	case AVCOL_PRI_FILM:
		return "Film";
	case AVCOL_PRI_BT2020:
		return "BT.2020";
	case AVCOL_PRI_SMPTE428:
		return "SMPTE.428";
	case AVCOL_PRI_SMPTE431:
		return "SMPTE.431";
	case AVCOL_PRI_SMPTE432:
		return "SMPTE.432";
	case AVCOL_PRI_EBU3213:
		return "EBU.3213";
	case AVCOL_PRI_NB:
		return "number";
	};

	return {};
}

std::string to_string(const AVColorTransferCharacteristic r) {
	return {};
}

std::string to_string(const AVColorSpace r) {
	return {};
}

std::string to_string(const AVColorRange r) {
	return {};
}

std::string to_string(const AVChromaLocation r) {
	return "";
}

std::string to_string(const AVPixelFormat r) {
	return {};
}

std::string streamType2String(const int type) {
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

std::string time2string(const int64_t t, const AVRational timebase) {
	LOGI("time is {} time base is {}, {}", t, timebase.den, timebase.num);
	std::string ret{};
	double ms = t * av_q2d(timebase);
	if (ms - int64_t(ms) > 0.000000001) {
		ret = std::to_string((ms - int64_t(ms)) * 1000) + TRANS_FETCH(kMSeconds) + ret;
	}
	int64_t n = ms;
	static const char *szs[] = { "second", "minute", "hour", "day" };
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

std::string size2String(const int64_t sz) {
	std::string ret{};
	auto n = sz;
	static const char *szs[] = { "byte ", "kB ", "mB ", "gB ", "pB " };
	int i = 0;
	while (n && i < 4) {
		ret = std::to_string(n % 1024) + szs[i % 5] + ret;
		n /= 1024;
		i++;
	}

	if (n) {
		ret = std::to_string(n) + szs[i % 5] + ret;
	}

	return ret.empty() ? "0byte" : ret;
}