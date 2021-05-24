/*

	externpx.c

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

// Apply a chain of n fLuts to an RGB pixel,
// process the result by an external function,
// then apply a second chain of m fLuts to
// whatever the external changes the result to.
KLBDC KOLIBA_PIXEL * KOLIBA_ExternalPixel(KOLIBA_PIXEL *pxout, const KOLIBA_PIXEL *pxin, const KOLIBA_FFLUT *fLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBAPIXELTOXYZ transformin, KOLIBAXYZTOPIXEL transformout) {
	KOLIBA_XYZ xyz;

	if ((pxout == NULL) || (pxin == NULL)) return NULL;

	if (transformin == NULL) {
		xyz.x = (double)pxin->red;
		xyz.y = (double)pxin->green;
		xyz.z = (double)pxin->blue;
	}
	else if(((transformin)(&xyz, pxin)) != &xyz) return NULL;

	if (KOLIBA_ExternalXyz(&xyz, &xyz, fLut, n, m, ext, params) == NULL) return NULL;

	if (transformout == NULL) {
		pxout->red   = (float)xyz.x;
		pxout->green = (float)xyz.y;
		pxout->blue  = (float)xyz.z;

		return pxout;
	}
	else return (((transformout)(pxout, &xyz)) == pxout) ? pxout : NULL;
}
