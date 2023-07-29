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



