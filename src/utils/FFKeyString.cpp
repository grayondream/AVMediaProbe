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

std::vector<std::string> to_string(const AVPixelFormat r) {
	switch(r){
        //{"YUV", "4:2:0", "Packed", "10", "Jpeg"}
		case AV_PIX_FMT_NONE:
            return {"None", "", "", "", ""};
		case AV_PIX_FMT_YUV420P:   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
            return {"YUV", "4:2:0", "Planner", "8", ""};
		case AV_PIX_FMT_YUYV422:   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
            return {"YUYV", "4:2:2", "Packed", "8", ""};
		case AV_PIX_FMT_RGB:     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
            return {"RGB", "8:8:8", "Packed", "8", ""};
		case AV_PIX_FMT_BGR24:     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
            return {"BGR", "8:8:8", "Packed", "8", ""};
		case AV_PIX_FMT_YUV422P:   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
            return {"YUV", "4:2:2", "Planner", "8", ""};
		case AV_PIX_FMT_YUV444P:   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
            return {"YUV", "4:4:4", "Planner", "8", ""};
		case AV_PIX_FMT_YUV410P:   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
            return {"YUV", "4:1:0", "Planner", "8", ""};
		case AV_PIX_FMT_YUV411P:   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
            return {"YUV", "4:1:1", "Planner", "8", ""};
		case AV_PIX_FMT_GRAY8:     ///<        Y        ,  8bpp
            return {"Gray", "", "", "8", ""};
		case AV_PIX_FMT_MONOWHITE: ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
            return {"MONOWHITE", "", "", "8", ""};
		case AV_PIX_FMT_MONOBLACK: ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
            return {"MONOBLACK", "", "", "8", ""};
		case AV_PIX_FMT_PAL8:      ///< 8 bits with AV_PIX_FMT_RGB32 palette
            return {"PAL", "8:8:8:8", "Packed", "8", ""};
		case AV_PIX_FMT_YUVJ420P:  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV420P and setting color_range
            return {"YUV", "4:2:0", "Planner", "8", "Jpeg"};
		case AV_PIX_FMT_YUVJ422P:  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV422P and setting color_range
            return {"YUV", "4:2:2", "Planner", "", "Jpeg"};
		case AV_PIX_FMT_YUVJ444P:  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV444P and setting color_range
            return {"YUV", "4:4:4", "Planner", "8", ""};
		case AV_PIX_FMT_UYVY422:   ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
            return {"UYVY", "4:2:2", "Packed", "8", ""};
		case AV_PIX_FMT_UYYVYY411: ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
            return {"UYYVYY", "4:1:1", "Packed", "8", ""};
		case AV_PIX_FMT_BGR8:      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR4:      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR4_BYTE: ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB8:      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB4:      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB4_BYTE: ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV12:      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV21:      ///< as above, but U and V bytes are swapped
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_ARGB:      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGBA:      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_ABGR:      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGRA:      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY16BE:  ///<        Y        , 16bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY16LE:  ///<        Y        , 16bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV440P:   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVJ440P:  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV440P and setting color_range
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P:  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB48BE:   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB48LE:   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB565BE:  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB565LE:  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB555BE:  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), big-endian   , X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB555LE:  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), little-endian, X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR565BE:  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR565LE:  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR555BE:  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), big-endian   , X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR555LE:  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), little-endian, X=unused/undefined
			return { "", """;YUV", "::", "Planner" , ""};
		case AV_PIX_FMT_VAAPI:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P16LE:  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P16BE:  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P16LE:  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P16BE:  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P16LE:  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P16BE:  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_DXVA2_VLD:    ///< HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB444LE:  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), little-endian, X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB444BE:  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), big-endian,    X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR444LE:  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), little-endian, X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR444BE:  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), big-endian,    X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YA8:       ///< 8 bits gray, 8 bits alpha
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR48BE:   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR48LE:   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P9BE: ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P9LE: ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P10BE:///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P10LE:///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P10BE:///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P10LE:///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P9BE: ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P9LE: ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P10BE:///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P10LE:///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P9BE: ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P9LE: ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP:      ///< planar GBR 4:4:4 24bpp
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP9BE:   ///< planar GBR 4:4:4 27bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP9LE:   ///< planar GBR 4:4:4 27bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP10BE:  ///< planar GBR 4:4:4 30bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP10LE:  ///< planar GBR 4:4:4 30bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP16BE:  ///< planar GBR 4:4:4 48bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP16LE:  ///< planar GBR 4:4:4 48bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P:  ///< planar YUV 4:2:2 24bpp, (1 Cr & Cb sample per 2x1 Y & A samples)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P:  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P9BE:  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P9LE:  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P9BE:  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P9LE:  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P9BE:  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P9LE:  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P10BE: ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P10LE: ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P10BE: ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P10LE: ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P10BE: ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P10LE: ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P16BE: ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA420P16LE: ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P16BE: ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P16LE: ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P16BE: ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P16LE: ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_VDPAU:     ///< HW acceleration through VDPAU, Picture.data[3] contains a VdpVideoSurface
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_XYZ12LE:      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as little-endian, the 4 lower bits are set to 0
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_XYZ12BE:      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as big-endian, the 4 lower bits are set to 0
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV16:         ///< interleaved chroma YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV20LE:       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV20BE:       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGBA64BE:     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGBA64LE:     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGRA64BE:     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGRA64LE:     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YVYU422:   ///< packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YA16BE:       ///< 16 bits gray, 16 bits alpha (big-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YA16LE:       ///< 16 bits gray, 16 bits alpha (little-endian)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP:        ///< planar GBRA 4:4:4:4 32bpp
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP16BE:    ///< planar GBRA 4:4:4:4 64bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP16LE:    ///< planar GBRA 4:4:4:4 64bpp, little-endian
			return {"YUV", "::", "Planner", ""};
		case AV_PIX_FMT_QSV:
			return { "YUV", "::", "Planner" , ""};
		case AV_PIX_FMT_MMAL:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_D3D11VA_VLD:  ///< HW decoding through Direct3D11 via old API, Picture.data[3] contains a ID3D11VideoDecoderOutputView pointer
			return { "YUV", "::", "Planner" , ""};
		case AV_PIX_FMT_CUDA:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_0RGB:        ///< packed RGB 8:8:8, 32bpp, XRGBXRGB...   X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_RGB0:        ///< packed RGB 8:8:8, 32bpp, RGBXRGBX...   X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_0BGR:        ///< packed BGR 8:8:8, 32bpp, XBGRXBGR...   X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BGR0:        ///< packed BGR 8:8:8, 32bpp, BGRXBGRX...   X=unused/undefined
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P12BE: ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P12LE: ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P14BE: ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV420P14LE: ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P12BE: ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P12LE: ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P14BE: ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV422P14LE: ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P12BE: ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P12LE: ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P14BE: ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV444P14LE: ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP12BE:    ///< planar GBR 4:4:4 36bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP12LE:    ///< planar GBR 4:4:4 36bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP14BE:    ///< planar GBR 4:4:4 42bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRP14LE:    ///< planar GBR 4:4:4 42bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVJ411P:    ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV411P and setting color_range
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_BGGR8:    ///< bayer, BGBG..(odd line), GRGR..(even line), 8-bit samples
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_RGGB8:    ///< bayer, RGRG..(odd line), GBGB..(even line), 8-bit samples
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GBRG8:    ///< bayer, GBGB..(odd line), RGRG..(even line), 8-bit samples
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GRBG8:    ///< bayer, GRGR..(odd line), BGBG..(even line), 8-bit samples
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_BGGR16LE: ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_BGGR16BE: ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_RGGB16LE: ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_RGGB16BE: ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GBRG16LE: ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GBRG16BE: ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GRBG16LE: ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_BAYER_GRBG16BE: ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_XVMC:///< XVideo Motion Acceleration via common packet passing
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV440P10LE: ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV440P10BE: ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV440P12LE: ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUV440P12BE: ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_AYUV64LE:    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_AYUV64BE:    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_VIDEOTOOLBOX: ///< hardware decoding through Videotoolbox
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_P010LE: ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_P010BE: ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP12BE:  ///< planar GBR 4:4:4:4 48bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP12LE:  ///< planar GBR 4:4:4:4 48bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP10BE:  ///< planar GBR 4:4:4:4 40bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAP10LE:  ///< planar GBR 4:4:4:4 40bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_MEDIACODEC: ///< hardware decoding through MediaCodec
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY12BE:   ///<        Y        , 12bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY12LE:   ///<        Y        , 12bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY10BE:   ///<        Y        , 10bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY10LE:   ///<        Y        , 10bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_P016LE: ///< like NV12, with 16bpp per component, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_P016BE: ///< like NV12, with 16bpp per component, big-endian
			return { "YUV", "::", "Planner" , ""};
		case AV_PIX_FMT_D3D11:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY9BE:   ///<        Y        , 9bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY9LE:   ///<        Y        , 9bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRPF32BE:  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRPF32LE:  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAPF32BE: ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GBRAPF32LE: ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, little-endian
			return { "YUV, """, "::", "Planner" , ""};
		case AV_PIX_FMT_DRM_PRIME:
			return { ", ""Y;UV", "::", "Planner" , ""};
		case AV_PIX_FMT_OPENCL:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY14BE:   ///<        Y        , 14bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAY14LE:   ///<        Y        , 14bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAYF32BE:  ///< IEEE-754 single precision Y, 32bpp, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_GRAYF32LE:  ///< IEEE-754 single precision Y, 32bpp, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P12BE: ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA422P12LE: ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P12BE: ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_YUVA444P12LE: ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, little-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV24:      ///< planar YUV 4:4:4, 24bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_NV42:      ///< as above, but U and V bytes are swapped
			return { ", ""Y;UV", "::", "Planner" , ""};
		case AV_PIX_FMT_VULKAN:
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_Y210BE:    ///< packed YUV 4:2:2 like YUYV422, 20bpp, data in the high bits, big-endian
            return {"YUV", "::", "Planner", "", ""};
		case AV_PIX_FMT_Y210LE:    ///< packed YUV 4:2:2 like YUYV422, 20bpp, data in the high bits, little-endian
            return {"YUV", "::", "Planner", "", ""};
		//case AV_PIX_FMT_X2RGB10LE: ///< packed RGB 10:10:10, 30bpp, (msb)2X 10R 10G 10B(lsb), little-endian, X=unused/undefined
        //    return {"YUV", "::", "Planner", ""};
		//case AV_PIX_FMT_X2RGB10BE: ///< packed RGB 10:10:10, 30bpp, (msb)2X 10R 10G 10B(lsb), big-endian, X=unused/undefined
        //    return {"YUV", "::", "Planner", ""};
		case AV_PIX_FMT_NB:        ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
            return {"YUV", "::", "Planner", ""}
    }
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