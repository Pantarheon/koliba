/*

	mkpslut.c

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

// Given a (*KOLIBA_MAKEVERTEX)() function, create
// a partial sLut, i.e., the sLut[i][j][k] portion
// of a large sLut.
KLBDC KOLIBA_SLUT * KOLIBA_MakePartialSlut(KOLIBA_SLUT * sLut, const unsigned int dim[3], const unsigned int ind[3], KOLIBA_MAKEVERTEX fn, const void * const params) {
	KOLIBA_RGB rgb;
	KOLIBA_RGB *f;

	if ((sLut == NULL) || (dim == NULL) || (ind == NULL) || (fn == NULL) ||
		(dim[0] > 256) || (ind[0] >= dim[0]) ||
		(dim[1] > 256) || (ind[1] >= dim[1]) ||
		(dim[2] > 256) || (ind[2] >= dim[2]))
			return NULL;

	rgb.r = (double)(ind[0]) / (double)(dim[0]);
	rgb.g = (double)(ind[1]) / (double)(dim[1]);
	rgb.b = (double)(ind[2]) / (double)(dim[2]);

	f = (KOLIBA_RGB *)sLut;
	(fn)(f++, &rgb, params);	// Black

	rgb.b = (double)(ind[2] + 1) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Blue

	rgb.g = (double)(ind[1] + 1) / (double)(dim[1]);
	rgb.b = (double)(ind[2]) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Green

	rgb.b = (double)(ind[2] + 1) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Cyan

	rgb.r = (double)(ind[0] + 1) / (double)(dim[0]);
	rgb.g = (double)(ind[1]) / (double)(dim[1]);
	rgb.b = (double)(ind[2]) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Red

	rgb.b = (double)(ind[2] + 1) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Magenta

	rgb.g = (double)(ind[1] + 1) / (double)(dim[1]);
	rgb.b = (double)(ind[2]) / (double)(dim[2]);
	(fn)(f++, &rgb, params);	// Yellow

	rgb.b = (double)(ind[2] + 1) / (double)(dim[2]);
	(fn)(f, &rgb, params);	// White

	return sLut;
}

