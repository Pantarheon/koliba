/*

	vertpixl.c

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

// Conversion between an array KOLIBA_VERTEX and KOLIBA_RGB.

KLBDC KOLIBA_PIXEL * KOLIBA_VertexToPixel(KOLIBA_PIXEL * p, const KOLIBA_VERTEX * const v, unsigned int n) {
	KOLIBA_PIXEL *P;
	const KOLIBA_VERTEX *V;

	if ((p == NULL) || (v == NULL) || (n == 0)) return NULL;

	P = p;
	V = v;

	while (n--) {
		P->red   = (float)V->r;
		P->green = (float)V->g;
		P->blue  = (float)V->b;
		P++; V++;
	}

	return p;
}

KLBDC KOLIBA_VERTEX * KOLIBA_PixelToVertex(KOLIBA_VERTEX *v, const KOLIBA_PIXEL * const p, unsigned int n) {
	const KOLIBA_PIXEL *P;
	KOLIBA_VERTEX *V;


	if ((v == NULL) || (p == NULL) || (n == 0)) return NULL;

	P = p;
	V = v;

	while (n--) {
		V->r = (double)P->red;
		V->g = (double)P->green;
		V->b = (double)P->blue;
		P++; V++;
	}

	return v;
}

// Compare whether the values in the vertex are the same as those
// in the pixel converted to doubles.
KLBDC int KOLIBA_PixelIsVertex(const KOLIBA_VERTEX * const v, const KOLIBA_PIXEL * const p) {
	if ((v == NULL) || (p == NULL)) return 0;
	else return (
		(v->r == (double)p->red)   &&
		(v->g == (double)p->green) &&
		(v->b == (double)p->blue)
	);
}

