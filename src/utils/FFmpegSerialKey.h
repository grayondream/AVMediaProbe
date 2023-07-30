#pragma once

#define CONSTEXPR_EXPR(key, value) constexpr char *key = #value;

/*
 * ths media json be like
 {
	"media" : {
		"filename" : str,
		"duration" : int,
		"
	},
	"1" : {
		"type" : "video"
	},
	"2" : {
		"type" : "audio"
	},
	...
 }


 */
CONSTEXPR_EXPR(kMedia, media);
CONSTEXPR_EXPR(kFileName, filename);
CONSTEXPR_EXPR(kDuration, duration);
CONSTEXPR_EXPR(kFormat, format);
CONSTEXPR_EXPR(kMediaFormat, media format);
CONSTEXPR_EXPR(kBitRate, bitrate);
CONSTEXPR_EXPR(kFps, fps);
CONSTEXPR_EXPR(kStartTime, start time);
CONSTEXPR_EXPR(kFormatScore, format score);

CONSTEXPR_EXPR(kStreamIndex, index);
CONSTEXPR_EXPR(kStreamType, type);
CONSTEXPR_EXPR(kStreamCodec, codec);
CONSTEXPR_EXPR(kVideoWidth, width);
CONSTEXPR_EXPR(kVideoHeight, height);
CONSTEXPR_EXPR(kVideoColorPri, color primaries);
CONSTEXPR_EXPR(kVideoColorSpace, color space);
CONSTEXPR_EXPR(kVideoColorTrc, color trc);
CONSTEXPR_EXPR(kVideoColorRange, color range);


