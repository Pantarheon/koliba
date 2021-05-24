/*

	polyxyz.c

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

// Apply a chain of up to n fLuts to a xyz.
// If n is zero or FFLUT is NULL, just copy
// the input to the output.
//
// If n is not zero, apply up to n fLuts, but
// break out of the loop early if any fLut[i]
// points at a NULL fLut.
KLBDC KOLIBA_XYZ * KOLIBA_PolyXyz(KOLIBA_XYZ * xyzout, const KOLIBA_XYZ * const xyzin, const KOLIBA_FFLUT * const fLut, unsigned int n) {
	KOLIBA_XYZ temp;
	double x, y, z, xy, xz, yz, xyz;
	const KOLIBA_FLUT *sLut;
	unsigned int flags;
	unsigned int i;

	if ((xyzin == NULL) || (xyzout == NULL)) return NULL;

	else if ((n == 0) || (fLut == NULL)) {
		xyzout->x = xyzin->x;
		xyzout->y = xyzin->y;
		xyzout->z = xyzin->z;
	}

	else {	// n != 0 && no NULL pointers so far
		temp.x = xyzin->x;
		temp.y = xyzin->y;
		temp.z = xyzin->z;

		for (i = 0; i < n; i++) {
			if ((sLut = fLut[i].fLut) == NULL) break;

			flags	= fLut[i].flags;
			x		= temp.x;
			y		= temp.y;
			z		= temp.z;

			temp.x = (flags & KOLIBA_KR) ? sLut->black.r : 0.0;
			temp.y = (flags & KOLIBA_KG) ? sLut->black.g : 0.0;
			temp.z = (flags & KOLIBA_KB) ? sLut->black.b : 0.0;

			if ((flags &(~KOLIBA_RgbFlutFlags)) == 0) continue;

			if (flags & KOLIBA_RR)
				temp.x += sLut->red.r * x;
			if (flags & KOLIBA_RG)
				temp.y += sLut->red.g * x;
			if (flags & KOLIBA_RB)
				temp.z += sLut->red.b * x;

			if (flags & KOLIBA_GR)
				temp.x += sLut->green.r * y;
			if (flags & KOLIBA_GG)
				temp.y += sLut->green.g * y;
			if (flags & KOLIBA_GB)
				temp.z += sLut->green.b * y;

			if (flags & KOLIBA_BR)
				temp.x += sLut->blue.r * z;
			if (flags & KOLIBA_BG)
				temp.y += sLut->blue.g * z;
			if (flags & KOLIBA_BB)
				temp.z += sLut->blue.b * z;

			if ((flags & (~KOLIBA_MatrixFlutFlags)) == 0) continue;

			if (flags & (KOLIBA_YR|KOLIBA_YG|KOLIBA_YB | KOLIBA_WR|KOLIBA_WG|KOLIBA_WB))
			xy	= x*y;

			if (flags & KOLIBA_YR)
				temp.x += sLut->yellow.r * xy;
			if (flags & KOLIBA_YG)
				temp.y += sLut->yellow.g * xy;
			if (flags & KOLIBA_YB)
				temp.z += sLut->yellow.b * xy;

			if (flags & (KOLIBA_MR|KOLIBA_MG|KOLIBA_MB))
				xz	= x*z;
			if (flags & KOLIBA_MR)
				temp.x += sLut->magenta.r * xz;
			if (flags & KOLIBA_MG)
				temp.y += sLut->magenta.g * xz;
			if (flags & KOLIBA_MB)
				temp.z += sLut->magenta.b * xz;

			if (flags & (KOLIBA_CR|KOLIBA_CG|KOLIBA_CB))
				yz = y*z;
			if (flags & KOLIBA_CR)
				temp.x += sLut->cyan.r * yz;
			if (flags & KOLIBA_CG)
				temp.y += sLut->cyan.g * yz;
			if (flags & KOLIBA_CB)
				temp.z += sLut->cyan.b * yz;

			if (flags & (KOLIBA_WR|KOLIBA_WG|KOLIBA_WB))
				xyz	= xy*z;
			if (flags & KOLIBA_WR)
				temp.x += sLut->white.r * xyz;
			if (flags & KOLIBA_WG)
				temp.y += sLut->white.g * xyz;
			if (flags & KOLIBA_WB)
				temp.z += sLut->white.b * xyz;
		}

		xyzout->x	= temp.x;
		xyzout->y	= temp.y;
		xyzout->z	= temp.z;
	}

	return xyzout;
}

