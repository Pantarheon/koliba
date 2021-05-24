/*

	fltcube.c

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

// Create a FLUT from a cube. The cube is
// an array of (m+1)(n+1)(o+1) KOLIBA_RGBs.
// We are given m, n, o, as well as i, j, k,
// where i is the red from m, j is the green
// from n, and k is the blue from o.
//
// And while the FLUT is dimensioned [m][n][o],
// the cube is dimensioned [o][n][m] for some
// reasons.
//
// The m, n, o dimensions are passed as an
// array of three integers. The i, j, k indices
// are passed as another array of three integers.
// The cube entry is const, but I removed the const
// qualifier from it because compilers get postal
// if I keep it in. :(
KLBDC KOLIBA_FLUT * KOLIBA_ConvertCubeToFlut(KOLIBA_FLUT * fLut, const KOLIBA_RGB * const cube, const unsigned int dim[3], const unsigned int ind[3]) {
	KOLIBA_VERTICES vert;
	KOLIBA_RGB *cb;
	unsigned int i, j, k, m, n, P;

	if ((cube == NULL) || (fLut == NULL) || (ind == NULL) || (dim == NULL) ||
	(((m = dim[0]) <= (i = ind[0])) || ((n = dim[1]) <= (j = ind[1])) || (dim[2] <= (k = ind[2])))) return NULL;

	// Find the starting address inside the cube corresponding
	// to the starting point inside the FLUT.
	//
	// i is the red position, j green, k blue. m is the red
	// dimension, n green, o blue (albeit not needed here).
	// Since in a cube i grows the fastest and k the slowest,
	// P = (k*(n+1) + j)(m+1) + i.

	cb = (KOLIBA_RGB *)cube;

	m++; n++;

	P = (k * (n) + j)*(m) + i;

	n *= m;

	vert.black   = cb+P;
	vert.blue    = cb+P+n;
	vert.green   = cb+P+m;
	vert.cyan    = cb+P+m+n;
	P++;
	vert.red     = cb+P;
	vert.magenta = cb+P+n;
	vert.yellow  = cb+P+m;
	vert.white   = cb+P+m+n;

	return KOLIBA_ConvertSlutToFlut(fLut, &vert);
}


