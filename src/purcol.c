/*

	purcol.c

	Copyright 2019 G. Adam Stanislav
	All rights reserved

	Redistribution and use in source and binary forms,
	with or without modification, are permitted provided
	that the following conditions are met:

	1. Redistributions of source code must retain the
	above copyright notice, this list of conditions
	and the following disclaimer.

	2. Redistributions in binary form must reproduce the
	above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or
	other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the
	names of its contributors may be used to endorse or
	promote products derived from this software without
	specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "koliba.h"

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// The "pure color" effect. Good for showing compression
// flaws in a photo or video. And for producing pure color
// (i.e., one with 100% saturation, except when the input
// is a shade of gray, which shall be left unchanged).
//
// Its "params" can either be NULL or be a KOLIBA_RGB
// structure with red, green and blue impurities.
KLBDC KOLIBA_RGB * KOLIBA_PureColor(KOLIBA_RGB * rgbout, const KOLIBA_RGB * const rgbin, const void * const params) {
	KOLIBA_RGB rgb;
	KOLIBA_RGB *impurity;

	if ((rgbout == NULL) || (rgbin == NULL)) return NULL;

	rgb.r = rgbin->r;
	rgb.g = rgbin->g;
	rgb.b = rgbin->b;

	if ((rgb.b == rgb.g) && (rgb.b == rgb.r)) {
		rgbout->r = rgb.r;
		rgbout->g = rgb.g;
		rgbout->b = rgb.b;

		return rgbout;
	}
	else if ((rgb.r >= rgb.g) && (rgb.r >= rgb.b)) {
		if (rgb.g == rgb.b) {
			rgb.g = 0.0;
			rgb.b = 0.0;
		}
		else if (rgb.g > rgb.b) rgb.b = 0.0;
		else rgb.g = 0;
	}
	else if ((rgb.g >= rgb.b) && (rgb.g >= rgb.r)) {
		if (rgb.b == rgb.r) {
			rgb.r = 0.0;
			rgb.b = 0.0;
		}
		else if (rgb.b > rgb.r) rgb.r = 0.0;
		else rgb.b = 0.0;
	}
	else if (rgb.g == rgb.r) {
		rgb.r = 0.0;
		rgb.g = 0.0;
	}
	else if (rgb.g > rgb.r) rgb.r = 0.0;
	else rgb.g = 0.0;

	if ((impurity = (KOLIBA_RGB *)params)!= NULL) {
		rgbout->r = (rgbin->r - rgb.r) * impurity->r + rgb.r;
		rgbout->g = (rgbin->g - rgb.g) * impurity->g + rgb.g;
		rgbout->b = (rgbin->b - rgb.b) * impurity->b + rgb.b;
	}
	else {
		rgbout->r = rgb.r;
		rgbout->g = rgb.g;
		rgbout->b = rgb.b;
	}

	return rgbout;
}

