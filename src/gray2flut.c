/*

	gray2flut.c

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

// Convert grayscale factors to a fLut. If the factors are NULL,
// use Rec. 2020 instead. If the factors add up to 0, set them to 1/3.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertGrayToFlut(KOLIBA_FLUT * fLut, const KOLIBA_RGB * const gray) {
	KOLIBA_RGB rec;
	double d;

	if (fLut != NULL) {
		if ((gray == NULL) || (gray == &KOLIBA_Rec2020)) memcpy(&rec, &KOLIBA_Rec2020, sizeof(KOLIBA_RGB));
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
		}

		fLut->black.r   = 0.0;
		fLut->black.g   = 0.0;
		fLut->black.b   = 0.0;

		fLut->red.r     = rec.r;
		fLut->red.g     = rec.r;
		fLut->red.b     = rec.r;

		fLut->green.r   = rec.g;
		fLut->green.g   = rec.g;
		fLut->green.b   = rec.g;

		fLut->blue.r    = rec.b;
		fLut->blue.g    = rec.b;
		fLut->blue.b    = rec.b;

		fLut->yellow.r  = 0.0;
		fLut->yellow.g  = 0.0;
		fLut->yellow.b  = 0.0;

		fLut->magenta.r = 0.0;
		fLut->magenta.g = 0.0;
		fLut->magenta.b = 0.0;

		fLut->cyan.r    = 0.0;
		fLut->cyan.g    = 0.0;
		fLut->cyan.b    = 0.0;

		fLut->white.r   = 0.0;
		fLut->white.g   = 0.0;
		fLut->white.b   = 0.0;
	}

	return fLut;
}
