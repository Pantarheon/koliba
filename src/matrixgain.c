/*

	matrixgain.c

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

KLBHID extern const double KOLIBA_Zeroes[24];

// Gain the channels of a matrix, each channel separately.
// If a modificand matrix is not specified, use I as input.
KLBDC KOLIBA_MATRIX * KOLIBA_MatrixGain(KOLIBA_MATRIX * output, const KOLIBA_MATRIX * const input, const KOLIBA_VERTEX * const gains) {
	KOLIBA_MATRIX temp;
	const KOLIBA_MATRIX *i;

	if (output == NULL) return output;

	i = (input != NULL) ? input : &KOLIBA_IdentityMatrix;

	KOLIBA_Interpolate((double *)&temp.red, (double *)&(i->red), (gains != NULL) ? gains->r : 1.0, KOLIBA_Zeroes, 3);
	temp.red.o = i->red.o;

	KOLIBA_Interpolate((double *)&temp.green, (double *)&(i->green), (gains != NULL) ? gains->g : 1.0, KOLIBA_Zeroes, 3);
	temp.green.o = i->green.o;

	KOLIBA_Interpolate((double *)&temp.blue, (double *)&(i->blue), (gains != NULL) ? gains->b : 1.0, KOLIBA_Zeroes, 3);
	temp.blue.o = i->blue.o;

	return memcpy(output, &temp, sizeof(KOLIBA_MATRIX));
}

