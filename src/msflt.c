/*

	msflt.c

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

// This is the inverse of KOLIBA_ConvertMatrixToFlut
// but only if the fLut really is a matrix.
//
// Otherwise, it extracts a matrix based on prime
// farba but ignoring secondary farba, which is not
// an inverse of KOLIBA_ConvertMatrixToFlut, but perhaps
// could be used as a special effect.

KLBDC KOLIBA_MATRIX * KOLIBA_ConvertFlutToMatrix(KOLIBA_MATRIX * mat, const KOLIBA_FLUT * const fLut) {
	if ((fLut == NULL) || (mat == NULL)) return NULL;

	mat->red.r   = fLut->red.r;
	mat->red.g   = fLut->green.r;
	mat->red.b   = fLut->blue.r;
	mat->red.o   = fLut->black.r;

	mat->green.r = fLut->red.g;
	mat->green.g = fLut->green.g;
	mat->green.b = fLut->blue.g;
	mat->green.o = fLut->black.g;

	mat->blue.r  = fLut->red.b;
	mat->blue.g  = fLut->green.b;
	mat->blue.b  = fLut->blue.b;
	mat->blue.o  = fLut->black.b;

	return mat;
}
