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
CONSTEXPR_EXPR(kStreamFrames, z frames);
CONSTEXPR_EXPR(kStreamFramesNumber, z frame number);
CONSTEXPR_EXPR(kStreamFrame, a frame);
CONSTEXPR_EXPR(kStreamSize, d f stream size);

CONSTEXPR_EXPR(kVideoWidth, f width);
CONSTEXPR_EXPR(kVideoHeight, f height);
CONSTEXPR_EXPR(kVideoTransMatrix, f zamatrix);
CONSTEXPR_EXPR(kVideoTransMatrixabu, [a b u]);
CONSTEXPR_EXPR(kVideoTransMatrixcdv, [c d v]);
CONSTEXPR_EXPR(kVideoTransMatrixxyw, [x y w]);

CONSTEXPR_EXPR(kVideoRatation, f zarotate);
CONSTEXPR_EXPR(kVideoFlipX, f zflipx);
CONSTEXPR_EXPR(kVideoFlipY, f zflipy);
CONSTEXPR_EXPR(kVideoSar, f zsar);
CONSTEXPR_EXPR(kVideoDar, f zdar);
CONSTEXPR_EXPR(kVideoPar, f zpar);

CONSTEXPR_EXPR(kVideoIsVFR, fa isvfr);
CONSTEXPR_EXPR(kVideoAVGFps, fe avgfps);
CONSTEXPR_EXPR(kVideoMAXFps, ff maxfps);

CONSTEXPR_EXPR(kVideoFmtFmt, ka pixfmt);
CONSTEXPR_EXPR(kVideoFmtLine, kb linesize);
CONSTEXPR_EXPR(kVideoFmtBit, kc bit);
CONSTEXPR_EXPR(kVideoFmtLayout, kd fmt layout);
CONSTEXPR_EXPR(kVideoFmtEnd, ke end);
CONSTEXPR_EXPR(kVideoFmtBitFormat, kf bit format);

CONSTEXPR_EXPR(kVideoColorRange, qacolor range);
CONSTEXPR_EXPR(kVideoColorPri, q color primaries);
CONSTEXPR_EXPR(kVideoColorSpace, q color space);
CONSTEXPR_EXPR(kVideoColorTrc, q color trc);

CONSTEXPR_EXPR(kAudioSampltRate, h sample rate);
CONSTEXPR_EXPR(kAudioChannel, i channel);
CONSTEXPR_EXPR(kAudioChannelLayout, j layout);

CONSTEXPR_EXPR(kMSeconds, millseconds);
CONSTEXPR_EXPR(kSeconds, second);
CONSTEXPR_EXPR(kMinute, minute);
CONSTEXPR_EXPR(kHour, hour);
CONSTEXPR_EXPR(kDay, day);
CONSTEXPR_EXPR(kHz, khz);
CONSTEXPR_EXPR(kVFR, vfr);
CONSTEXPR_EXPR(kCFR, cfr);
CONSTEXPR_EXPR(kVideo, video);
CONSTEXPR_EXPR(kAudio, audio);
CONSTEXPR_EXPR(kSubtitle, subtitle);
CONSTEXPR_EXPR(kPixel, pixel);
CONSTEXPR_EXPR(kUnkonw, unkown);
CONSTEXPR_EXPR(kYes, yes);
CONSTEXPR_EXPR(kNo, no);

