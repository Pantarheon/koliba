/*

	chroma.c

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
#include <math.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Produce a matrix to control chroma angle, saturation
// and black and white levels.
//
// The angle has to be in degrees.

KLBDC KOLIBA_MATRIX * KOLIBA_ChromaMatrix(KOLIBA_MATRIX *output, const KOLIBA_CHROMA * const chroma, const KOLIBA_RGB * const model) {
	KOLIBA_MATRIX a, b, c;
	double angle;

	if ((chroma == NULL) || (model == NULL)) return NULL;

	angle = KOLIBA_ConvertDegreesToRadians(chroma->angle);
	c.red.r    =  chroma->white - chroma->black;
	c.red.g    =  0.0;
	c.red.b    =  0.0;
	c.red.o    =  chroma->black;
	c.green.r  =  0.0;
	c.green.g  =  cos(angle);
	c.green.b  =  sin(angle);
	c.green.o  =  0.0;
	c.blue.r   =  0.0;
	c.blue.g   = -c.green.b;
	c.blue.b   =  c.green.g;
	c.blue.o   =  0.0;
	KOLIBA_Interpolate((double*)&c.green, (double*)&c.green, chroma->magnitude, (double *)&KOLIBA_IdentityMatrix.green, 8);
	c.green.g *= chroma->saturation;
	c.green.b *= chroma->saturation;
	c.blue.g  *= chroma->saturation;
	c.blue.b  *= chroma->saturation;
	return KOLIBA_MultiplyMatrices(output, KOLIBA_MultiplyMatrices(output, KOLIBA_RgbToYcc(&b, model), &c), KOLIBA_YccToRgb(&a, model));
}


KLBDC KOLIBA_MATRIX * KOLIBA_ChromaticMatrix(KOLIBA_MATRIX * output, const KOLIBA_CHROMAT * const chromat) {
	if (chromat == NULL) return NULL;
	else return KOLIBA_ChromaMatrix(output, &(chromat->chroma), &(chromat->model));
}
