/*

	apppx.c

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

// Apply a fLut to a pixel.

KLBDC KOLIBA_PIXEL * KOLIBA_ApplyPixel(KOLIBA_PIXEL *pxout, const KOLIBA_PIXEL *pxin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBAPIXELTOXYZ transformin, KOLIBAXYZTOPIXEL transformout) {
	KOLIBA_XYZ xyz;

	if ((pxin == NULL) || (pxout == NULL)) return NULL;

	if (transformin == NULL) {
		xyz.x = (double)pxin->red;
		xyz.y = (double)pxin->green;
		xyz.z = (double)pxin->blue;
	}
	else if(((transformin)(&xyz, pxin)) != &xyz) return NULL;

	if (KOLIBA_ApplyXyz(&xyz, &xyz, fLut, flags) == NULL) return NULL;

	if (transformout == NULL) {
		pxout->red   = (float)xyz.x;
		pxout->green = (float)xyz.y;
		pxout->blue  = (float)xyz.z;

		return pxout;
	}
	else return (((transformout)(pxout, &xyz)) == pxout) ? pxout : NULL;
}

