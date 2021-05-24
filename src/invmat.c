/*

	invmat.c

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
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Invert a matrix, if you can.
KLBDC KOLIBA_MATRIX * KOLIBA_InvertMatrix(KOLIBA_MATRIX * output, const KOLIBA_MATRIX * const mat) {
	KOLIBA_MATRIX m;
	double det, a, b, c;
	unsigned int z = ((mat->red.o == 0.0) && (mat->green.o == 0.0) && (mat->blue.o == 0.0));
	// If z is true, our implied 4x4 matrix, is really just a 3x3 matrix.

#define	ma	(mat->red.r)
#define	mb	(mat->red.g)
#define	mc	(mat->red.b)
#define	md	(mat->green.r)
#define	me	(mat->green.g)
#define	mf	(mat->green.b)
#define	mg	(mat->blue.r)
#define	mh	(mat->blue.g)
#define	mi	(mat->blue.b)
#define	mj	(mat->red.o)
#define	mk	(mat->green.o)
#define	ml	(mat->blue.o)

#define	A	m.red.r
#define	B	m.green.r
#define	C	m.blue.r
#define	D	m.red.g
#define	E	m.green.g
#define	F	m.blue.g
#define	G	m.red.b
#define	H	m.green.b
#define	I	m.blue.b
#define	J	m.red.o
#define	K	m.green.o
#define L	m.blue.o


	if ((output == NULL) || (mat == NULL)) return NULL;

	A = me * mi - mf * mh;
	B = mf * mg - md * mi;
	C = md * mh - me * mg;
	D = mc * mh - mb * mi;
	G = mb * mf - mc * me;

	// Is it invertible? We are working with 3x4 matrices,
	// which imply a fourth row, like this:
	//
	//	a b c j
	//	d e f k
	//	g h i l
	//	0 0 0 1
	//
	// If j, k, and l all equal to 0, the matrix
	// is invertible if the 3x3 matrix a..i is invertible.
	// This is the case if the determinant is not 0.
	//
	// Indeed, given the implied fourth row, even the
	// 4x4 matrix happens to be mathematically
	// invertible if the 3x3 matrix is invertible,
	// but inverting such a 4x4 matrix may result
	// in the [4,4] element becoming something other
	// than 1.
	//
	// Since for our purposes we always imply the last
	// row to be [0 0 0 1], we will only consider such
	// a 4x4 matrix invertible, if the inversion keeps
	// that [0 0 0 1] implication. This is ascertained
	// by the condition after the || in the next line.
	if (((det = ma*A + mb*B + mc*C) == 0.0) || ((z == 0) && ((ma*A + md*D + mg*G) != det))) return NULL;
	det = 1.0 / det;

	A *= det;
	B *= det;
	C *= det;
	D *= det;
	G *= det;

	E = (ma*mi - mc*mg) * det;
	F = (mb*mg - ma*mh) * det;

	H = (mc*md - ma*mf) * det;
	I = (ma*me - mb*md) * det;

	if (z != 0) {
		J = 0.0;
		K = 0.0;
		L = 0.0;
	}
	else {
		a = mf*ml-mk*mi;
		b = mc*ml-mj*mi;
		c = mc*mk-mj*mf;

		J = (me*b - mb*a - mh*c) * det;
		K = (ma*a - md*b + mg*c) * det;
		L = (ma*(mk*mh-me*ml) + md*(mb*ml-mj*mh) + mg*(mj*me-mb*mk)) * det;
	}

	memcpy(output, &m, sizeof(KOLIBA_MATRIX));
	return output;
}
