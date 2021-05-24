/*

	yccrgb.c

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

// Create a matrix to convert YCbCr to RGB,
// where RGB values are between 0 and 1.
//
// We could just calculate the inverse of the
// RGB to YCbCr matrix, but it just so happens
// this type of matrix is much simpler to
// calculate. So why waste time and resources?
KLBDC KOLIBA_MATRIX * KOLIBA_YccToRgb(KOLIBA_MATRIX * output, const KOLIBA_RGB * const rec) {
	KOLIBA_MATRIX m;
	double d;

	if ((output == NULL) || (KOLIBA_RgbToYcc(&m, rec) == NULL) ||
	((d = m.green.g*m.blue.b-m.green.b*m.blue.g) == 0)) return NULL;

	// These are the only two values we have to
	// calculate using the matrix inverse algorithm.
	//
	// The determinant also happens to be simpler
	// to calculate in this case than in
	// a general matrix inverse calculation,
	// and since we have just made sure it is non-0,
	// we are safe dividing the two values by it.
	output->green.g = (m.red.r*m.blue.b-m.red.b*m.blue.r)/d;
	output->green.b = (m.red.b*m.green.r-m.red.r*m.green.b)/d;

	// These are actually (1.0-m.red.r)*2.0 and
	// (1.0-m.red.b)*2.0 respectively, but this
	// should reduce round-off errors (yes, they
	// exist in doubles, too).
	output->red.b   = (m.red.g+m.red.b) * 2.0;
	output->blue.g  = (m.red.g+m.red.r) * 2.0;

	// The remaining eight always have these results,
	// so we just assign them as constants.
	output->red.g   = 0.0;
	output->red.o   = 0.0;
	output->green.o = 0.0;
	output->blue.b  = 0.0;
	output->blue.o  = 0.0;
	output->red.r   = 1.0;
	output->green.r = 1.0;
	output->blue.r  = 1.0;

	return output;
}
