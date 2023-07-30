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
CONSTEXPR_EXPR(kMedia, a media);
CONSTEXPR_EXPR(kStream, a stream);

CONSTEXPR_EXPR(kFileName, a filename);
CONSTEXPR_EXPR(kFilSize, a fsize);
CONSTEXPR_EXPR(kDuration, b duration);
CONSTEXPR_EXPR(kFormat, c format);
CONSTEXPR_EXPR(kMediaFormat, d media format);
CONSTEXPR_EXPR(kFormatScore, e format score);
CONSTEXPR_EXPR(kBitRate, f media bitrate);
CONSTEXPR_EXPR(kStartTime, g media start time);

CONSTEXPR_EXPR(kStreamIndex, a index);
CONSTEXPR_EXPR(kStreamType, b type);
CONSTEXPR_EXPR(kStreamCodec, c codec);
CONSTEXPR_EXPR(kFps, c fps);
CONSTEXPR_EXPR(kStreamBitRate, d bitrate);
CONSTEXPR_EXPR(kStreamDuration, d stream duration);
CONSTEXPR_EXPR(kStreamStartTime, e stream start time);

CONSTEXPR_EXPR(kVideoWidth, f width);
CONSTEXPR_EXPR(kVideoHeight, f height);
CONSTEXPR_EXPR(kVideoColorPri, h color primaries);
CONSTEXPR_EXPR(kVideoColorSpace, h color space);
CONSTEXPR_EXPR(kVideoColorTrc, h color trc);
CONSTEXPR_EXPR(kVideoColorRange, h color range);



CONSTEXPR_EXPR(kSeconds, second);
CONSTEXPR_EXPR(kMinute, minute);
CONSTEXPR_EXPR(kHour, hour);
CONSTEXPR_EXPR(kDay, day);

