/*

	xyz8abgr.c

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

// Convert XYZ to the RGB portion of ABGR8PIXEL.
// Since this is likely to be called millions of
// times, we are not wasting time checking for
// NULL pointers, and leave that responsibility
// up to the caller.
//
// oconv, however, may be NULL.
//
// NB: We do not set px->a to anything, so the caller
// can set it as needed, before or after calling us.
KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_XyzToAbgr8Pixel(KOLIBA_ABGR8PIXEL *px, const KOLIBA_XYZ * const xyz, const unsigned char * const oconv) {
	register unsigned char r, g, b;

	r = (xyz->x <= 0.0) ? 0 : (xyz->x >= 1.0) ? 255 : (unsigned)(xyz->x*255.0+0.5);
	g = (xyz->y <= 0.0) ? 0 : (xyz->y >= 1.0) ? 255 : (unsigned)(xyz->y*255.0+0.5);
	b = (xyz->z <= 0.0) ? 0 : (xyz->z >= 1.0) ? 255 : (unsigned)(xyz->z*255.0+0.5);

	px->r = (oconv) ? oconv[r] : r;
	px->g = (oconv) ? oconv[g] : g;
	px->b = (oconv) ? oconv[b] : b;

	return px;
}
