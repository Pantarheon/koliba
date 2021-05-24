/*

	tintflut.c

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

#ifndef	KOLIBCALLS
#define	KOLIBCALLS
#endif

#include "koliba.h"
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Convert a tint to a FLUT, either directly
// or via a SLUT, based on function arguments.
//
// While we are using a KOLIBA_VERTEX structure
// with its r, g, b members for the tint, these
// should be presented in the user interface as
// cyan, magenta, and yellow.
KLBDC KOLIBA_FLUT * KOLIBA_TintToFlut(KOLIBA_FLUT * f, const KOLIBA_VERTEX * const tint, double saturation, double tinge, double light, char invert, char flut, const KOLIBA_RGB * const rec) {
	KOLIBA_FLUT fLut;
	KOLIBA_VERTICES v;
	double white;

	if (f == NULL) return NULL;

	if (flut) KOLIBA_FlutEfficacy(&fLut, KOLIBA_ConvertRecToFlut(&fLut, rec), 1.0 - saturation);
	else KOLIBA_SlutEfficacy((KOLIBA_SLUT *)&fLut, KOLIBA_ConvertRecToSlut((KOLIBA_SLUT *)&fLut, rec), 1.0 - saturation);

	if (tint != NULL) {
		fLut.black.r = -(tint->r * tinge);
		fLut.black.g = -(tint->g * tinge);
		fLut.black.b = -(tint->b * tinge);

		white = (flut) ? 0.0 : 1.0;

		if (!invert) {
			fLut.white.r = white + (1.0 - tint->r) * light;
			fLut.white.g = white + (1.0 - tint->g) * light;
			fLut.white.b = white + (1.0 - tint->b) * light;
		}
		else {
			fLut.white.r = white + tint->r * light;
			fLut.white.g = white + tint->g * light;
			fLut.white.b = white + tint->b * light;
		}
	}

	if (!flut) {
		KOLIBA_SlutToVertices(&v, (KOLIBA_SLUT *)&fLut);
		return KOLIBA_ConvertSlutToFlut(f, &v);
	}
	else return memcpy(f, &fLut, sizeof(KOLIBA_FLUT));
}
