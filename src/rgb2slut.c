/*

	rgb2slut.c

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

KLBHID extern const KOLIBA_RGB KOLIBA_Zeroes[8];

// Convert RGB color to a sLut. If no color is specified, use black.
KLBDC KOLIBA_SLUT * KOLIBA_ConvertRgbToSlut(KOLIBA_SLUT * sLut, const KOLIBA_RGB * const rgb) {
	register KOLIBA_RGB color;
	KOLIBA_VERTEX *s;
	unsigned int i;

	if (sLut != NULL) {
		if (rgb == NULL) {
			color.r = KOLIBA_Zeroes->r;
			color.g = KOLIBA_Zeroes->g;
			color.b = KOLIBA_Zeroes->b;
		} else {
			color.r = rgb->r;
			color.g = rgb->g;
			color.b = rgb->b;
		}

		for (i = 8, s = (KOLIBA_VERTEX *)sLut; i; i--, s++) {
			s->r = color.r;
			s->g = color.g;
			s->b = color.b;
		}
	}

	return sLut;
}
