/*

	sltcfs.c

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


// Convert a color filter to a sLut.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertColorFilterToSlut(KOLIBA_SLUT * sLut, const KOLIBA_CFLT * const cFlt) {
	double r,g,b,d, R, G, B;

	if ((sLut == NULL) || (cFlt == NULL)) return NULL;

	d = cFlt->d;
	r = cFlt->r * d;
	g = cFlt->g * d;
	b = cFlt->b * d;
	d = 1.0 - d;
	R = r + d;
	G = g + d;
	B = b + d;

	sLut->black.r   = 0.0;
	sLut->black.g   = 0.0;
	sLut->black.b   = 0.0;
	sLut->blue.r    = r;
	sLut->blue.g    = g;
	sLut->blue.b    = B;
	sLut->green.r   = r;
	sLut->green.g   = G;
	sLut->green.b   = b;
	sLut->cyan.r    = r;
	sLut->cyan.g    = G;
	sLut->cyan.b    = B;
	sLut->red.r     = R;
	sLut->red.g     = g;
	sLut->red.b     = b;
	sLut->magenta.r = R;
	sLut->magenta.g = g;
	sLut->magenta.b = B;
	sLut->yellow.r  = R;
	sLut->yellow.g  = G;
	sLut->yellow.b  = b;
	sLut->white.r   = 1.0;
	sLut->white.g   = 1.0;
	sLut->white.b   = 1.0;

	return sLut;
}
