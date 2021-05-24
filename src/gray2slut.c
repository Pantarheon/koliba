/*

	gray2slut.c

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
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Convert grayscale factors to a sLut. If the factors are NULL,
// use Rec. 2020 instead. If the factors add up to 0, set them to 1/3.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertGrayToSlut(KOLIBA_SLUT * sLut, const KOLIBA_RGB * const gray) {
	KOLIBA_RGB rec;
	double d;

	if (sLut != NULL) {
		if ((gray == NULL) || (gray == &KOLIBA_Rec2020))
			memcpy(sLut, &KOLIBA_Rec2020Slut, sizeof(KOLIBA_SLUT));
		else {
			if ((d = gray->r + gray->g + gray->b) == 0.0) {
				rec.r = 1.0 / 3.0;
				rec.g = 1.0 / 3.0;
			}
			else {
				rec.r = gray->r / d;
				rec.g = gray->g / d;
			}

			rec.b = 1.0 - (rec.r + rec.g);

			sLut->black.r   = 0.0;
			sLut->black.g   = 0.0;
			sLut->black.b   = 0.0;

			sLut->blue.r    = rec.b;
			sLut->blue.g    = rec.b;
			sLut->blue.b    = rec.b;

			sLut->green.r   = rec.g;
			sLut->green.g   = rec.g;
			sLut->green.b   = rec.g;

			d               = rec.b + rec.g;
			sLut->cyan.r    = d;
			sLut->cyan.g    = d;
			sLut->cyan.b    = d;

			sLut->red.r     = rec.r;
			sLut->red.g     = rec.r;
			sLut->red.b     = rec.r;

			d               = rec.r + rec.b;
			sLut->magenta.r = d;
			sLut->magenta.g = d;
			sLut->magenta.b = d;

			d               = rec.r + rec.g;
			sLut->yellow.r  = d;
			sLut->yellow.g  = d;
			sLut->yellow.b  = d;

			sLut->white.r   = 1.0;
			sLut->white.g   = 1.0;
			sLut->white.b   = 1.0;
		}
	}

	return sLut;
}
