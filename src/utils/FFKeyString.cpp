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
    switch(r){
        case AVCOL_TRC_RESERVED0:
            return "reserved";
        case AVCOL_TRC_BT709:
            return "BT.709";
        case AVCOL_TRC_UNSPECIFIED:
            return "unspecified";
        case AVCOL_TRC_RESERVED:
            return "reserved";
        case AVCOL_TRC_GAMMA22:
            return "GAMMA.22";
        case AVCOL_TRC_GAMMA28:
            return "GAMMA.28";
        case AVCOL_TRC_SMPTE170M:
            return "SMPTE.170M";
        case AVCOL_TRC_SMPTE240M:
            return "SMPTE.240M";
        case AVCOL_TRC_LINEAR:
            return "Linear";
        case AVCOL_TRC_LOG:
            return "LOG";
        case AVCOL_TRC_LOG_SQRT:
            return "LOG.SQRT";
        case AVCOL_TRC_IEC61966_2_4:
            return "IEC61966.2.4";
        case AVCOL_TRC_BT1361_ECG:
            return "BT.1361.ECG";
        case AVCOL_TRC_IEC61966_2_1:
            return "IEC.61966.2.1";
        case AVCOL_TRC_BT2020_10:
            return "BT.2020.10";
        case AVCOL_TRC_BT2020_12:
            return "BT.2020.12";
        case AVCOL_TRC_SMPTE2084:
            return "SMPTE.2084";
        case AVCOL_TRC_SMPTE428:
            return "SMPTE.428";
        case AVCOL_TRC_ARIB_STD_B67:
            return "STD.B67";
        case AVCOL_TRC_NB:
            return "number";
    }

    return "";
}

std::string to_string(const AVColorSpace r) {
	switch(r){
        case AVCOL_SPC_RGB:
            return "RGB";
        case AVCOL_SPC_BT709:
            return "BT.709";
        case AVCOL_SPC_UNSPECIFIED:
            return "unspecified";
        case AVCOL_SPC_RESERVED:
            return "reserved";
        case AVCOL_SPC_FCC:
            return "FCC";
        case AVCOL_SPC_BT470BG:
            return "BT.470BG";
        case AVCOL_SPC_SMPTE170M:
            return "SMPTE.170M";
        case AVCOL_SPC_SMPTE240M:
            return "SMPTE.240M";
        case AVCOL_SPC_YCGCO:
            return "YCGCO";
        case AVCOL_SPC_BT2020_NCL:
            return "BT.2020.NCL";
        case AVCOL_SPC_BT2020_CL:
            return "BT.2020.CL";
        case AVCOL_SPC_SMPTE2085:
            return "SMPTE.2085";
        case AVCOL_SPC_CHROMA_DERIVED_NCL:
            return "CHROMA.DERIVED.NCL";
        case AVCOL_SPC_CHROMA_DERIVED_CL:
            return "CHROMA.DERIVED.CL";
        case AVCOL_SPC_ICTCP:
            return "ICTCP";
        case AVCOL_SPC_NB:
            return "number";
    }
    return {};
}

std::string to_string(const AVColorRange r) {
    switch(r){
        case AVCOL_RANGE_UNSPECIFIED:
            return "unspecified";
        case AVCOL_RANGE_MPEG:
            return "Full";
        case AVCOL_RANGE_JPEG:
            return "Limited";
        case AVCOL_RANGE_NB:
            return "number";
    }
	return {};
}

std::string to_string(const AVChromaLocation r) {
    switch(r){
        case AVCHROMA_LOC_UNSPECIFIED:
            return "unspecified";
        case AVCHROMA_LOC_LEFT:
            return "left";
        case AVCHROMA_LOC_CENTER:
            return "center";
        case AVCHROMA_LOC_TOPLEFT:
            return "top left";
        case AVCHROMA_LOC_TOP:
            return "top";
        case AVCHROMA_LOC_BOTTOMLEFT:
            return "bottom left";
        case AVCHROMA_LOC_BOTTOM:
            return "bottom";
        case AVCHROMA_LOC_NB:
            return "number";
    }

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