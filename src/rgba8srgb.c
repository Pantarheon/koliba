/*

	rgba8srgb.c

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
#include <math.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif


// Convert between 8bpp RGBA and sRGB.
KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8ToSrgb(KOLIBA_RGBA8PIXEL * pixelout, const KOLIBA_RGBA8PIXEL * const pixelin, unsigned int count) {
	unsigned char *p, *q;
	unsigned int i, j;

	if ((pixelin == NULL) || (pixelout == NULL)) return NULL;

	p = (unsigned char *)pixelin;
	q = (unsigned char *)pixelout;

	for (i = 0; i < count; i++) {
		for (j = 0; j < 3; j++) *q++  = KOLIBA_LinearByteToSrgb[*p++];
		*q++ = *p++;
	}

	return pixelout;
}

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_SrgbToRgba8(KOLIBA_RGBA8PIXEL * pixelout, const KOLIBA_RGBA8PIXEL * const pixelin, unsigned int count) {
	double d;
	unsigned char *p, *q;
	unsigned int i, j;

	if ((pixelin == NULL) || (pixelout == NULL)) return NULL;

	p = (unsigned char *)pixelin;
	q = (unsigned char *)pixelout;

	for (i = 0; i < count; i++) {
		for (j = 0; j < 3; j++, p++, q++) {
			d  = 255.0 * KOLIBA_SrgbToLinear((double)*p / 255.0);
			*q = (d >= 254.5) ? 255 : (d < 0.5) ? 0 : (unsigned char)((unsigned int)(d+0.5));
		}
		*q++ = *p++;
	}

	return pixelout;
}
