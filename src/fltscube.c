/*

	fltscube.c

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

// Create m*n*o FLUTs from a cuboid.
KLBDC KOLIBA_FLUT * KOLIBA_ConvertCubeToFluts(KOLIBA_FLUT * fLut, const KOLIBA_RGB * const cube, const unsigned int dim[3]) {
	unsigned int i, j, k, m, n, o, ind[3];
	KOLIBA_FLUT *f;

	if ((fLut == NULL) || (cube == NULL) || (dim == NULL) ||
	((m = dim[0]) == 0) || ((n = dim[1]) == 0) || ((o = dim[2]) == 0) ||
	(m > 256) || (n > 256) || (o > 256)) return NULL;

	for (i = 0, f = fLut; i < m; i++) {
		ind[0] = i;
		for (j = 0; j < n; j++) {
			ind[1] = j;
			for (k = 0; k < o; k++, f++) {
				ind[2] = k;
				KOLIBA_ConvertCubeToFlut(f, cube, dim, ind);
			}
		}
		
	}

	return fLut;
}
