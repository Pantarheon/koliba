/*

	appxyz.c

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

// Apply a fLut to a xyz.
// If fLut is NULL but we have valid
// output and input pointers, copy
// the input to the output.
KLBDC KOLIBA_XYZ * KOLIBA_ApplyXyz(KOLIBA_XYZ * xyzout, const KOLIBA_XYZ * const xyzin, const KOLIBA_FLUT * const fLut, KOLIBA_FLAGS flags) {
	KOLIBA_XYZ temp;
	double x, y, z, xy, xz, yz, xyz;

	if ((xyzin == NULL) || (xyzout == NULL)) return NULL;

	else if (fLut == NULL) {
		xyzout->x = xyzin->x;
		xyzout->y = xyzin->y;
		xyzout->z = xyzin->z;
	}

	else {
		x = (flags & KOLIBA_KR) ? xyzin->x : 0.0;
		y = (flags & KOLIBA_KG) ? xyzin->y : 0.0;
		z = (flags & KOLIBA_KB) ? xyzin->z : 0.0;

		if (flags & 0xE07000)
		xy	= x*y;

		temp.x = fLut->black.r;
		temp.y = fLut->black.g;
		temp.z = fLut->black.b;

		if (flags & 0x000008)
			temp.x += fLut->red.r * x;
		if (flags & 0x000010)
			temp.y += fLut->red.g * x;
		if (flags & 0x000020)
			temp.z += fLut->red.b * x;

		if (flags & 0x000040)
			temp.x += fLut->green.r * y;
		if (flags & 0x000080)
			temp.y += fLut->green.g * y;
		if (flags & 0x000100)
			temp.z += fLut->green.b * y;

		if (flags & 0x000200)
			temp.x += fLut->blue.r * z;
		if (flags & 0x000400)
			temp.y += fLut->blue.g * z;
		if (flags & 0x000800)
			temp.z += fLut->blue.b * z;

		if (flags & 0x001000)
			temp.x += fLut->yellow.r * xy;
		if (flags & 0x002000)
			temp.y += fLut->yellow.g * xy;
		if (flags & 0x004000)
			temp.z += fLut->yellow.b * xy;

		if (flags & 0x038000)
			xz	= x*z;
		if (flags & 0x008000)
			temp.x += fLut->magenta.r * xz;
		if (flags & 0x010000)
			temp.y += fLut->magenta.g * xz;
		if (flags & 0x020000)
			temp.z += fLut->magenta.b * xz;

		if (flags & 0x1C0000)
			yz = y*z;
		if (flags & 0x040000)
			temp.x += fLut->cyan.r * yz;
		if (flags & 0x080000)
			temp.y += fLut->cyan.g * yz;
		if (flags & 0x100000)
			temp.z += fLut->cyan.b * yz;

		if (flags & 0xE00000)
			xyz	= xy*z;
		if (flags & 0x200000)
			temp.x += fLut->white.r * xyz;
		if (flags & 0x400000)
			temp.y += fLut->white.g * xyz;
		if (flags & 0x800000)
			temp.z += fLut->white.b * xyz;

		xyzout->x	= temp.x;
		xyzout->y	= temp.y;
		xyzout->z	= temp.z;
	}

	return xyzout;
}

