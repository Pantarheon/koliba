/*

	mkcube.c

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

// Make a series of RGB values in the same order as used
// in the .cube files. Though this code is more general
// than what cubes do, so we can have different dimension
// for each axis. Let m be the dimension of the red (or X)
// axis, n the dimension of the green (or Y) axis, and o
// the dimension of the blue (or Z) axis.
//
// The caller must pass us a pointer to (m+1)*(n+1)*(o+1)
// KOLIBA_RGB structures, the values of m, n, o, and a
// pointer to a (*KOLIBA_MAKEVERTEX)() function and its
// parameters. We will call the function (m+1)(n+1)(o+1)
// times, each time passing to it a different input RGB
// structure.
//
// Please note that to produce values suitable to create
// a .cube file of p*p*p dimensions, you have to pass
// m = p-1, n = p-1, o = p-1, because cube files pass
// the number of needed vertices, not the number of cubes
// to make.
//
// The values of each dimension must be no less than 1 and
// no more than 256. To make a valid .cube file, the values
// must be no more than 255 and all three have to be the same.
KLBDC KOLIBA_RGB * KOLIBA_MakeCube(KOLIBA_RGB * cube, unsigned int m, unsigned int n, unsigned int o, KOLIBA_MAKEVERTEX fn, const void * const params) {
	KOLIBA_RGB rgb;
	unsigned int i, j, k;
	KOLIBA_RGB *ptr;

	if ((cube == NULL) || (fn == NULL) || (m == 0) || (n == 0) || (o == 0) || (m > 256) || (n > 256) || (o > 256))
		return NULL;

	for (i = 0, ptr = cube; i <= o; i++) {
		rgb.b = (double)i / (double)o;
		for (j = 0; j <= n; j++) {
			rgb.g = (double)j / (double)n;
			for (k = 0; k <= m; k++, ptr++) {
				rgb.r = (double)k / (double)m;
				(fn)(ptr, &rgb, params);
			}
		}
	}
	return cube;
}


