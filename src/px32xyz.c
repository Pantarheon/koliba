/*

	px32xyz.c

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

// Convert the RGB portion of RGBA32PIXEL to XYZ.
KLBDC KOLIBA_XYZ * KOLIBA_Rgba32PixelToXyz(KOLIBA_XYZ *xyz, const KOLIBA_RGBA32PIXEL * const px, KOLIBA_DBLCONV iconv) {
	xyz->x = (iconv) ? iconv((double)(px->r)) : (double)(px->r);
	xyz->y = (iconv) ? iconv((double)(px->g)) : (double)(px->g);
	xyz->z = (iconv) ? iconv((double)(px->b)) : (double)(px->b);

	return xyz;
}

// Convert the RGB portion of ARGB32PIXEL to XYZ.
KLBDC KOLIBA_XYZ * KOLIBA_Argb32PixelToXyz(KOLIBA_XYZ *xyz, const KOLIBA_ARGB32PIXEL * const px, KOLIBA_DBLCONV iconv) {
	xyz->x = (iconv) ? iconv((double)(px->r)) : (double)(px->r);
	xyz->y = (iconv) ? iconv((double)(px->g)) : (double)(px->g);
	xyz->z = (iconv) ? iconv((double)(px->b)) : (double)(px->b);

	return xyz;
}
