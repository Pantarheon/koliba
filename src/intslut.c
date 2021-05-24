/*

	intslut.c

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

// Conditionally interpolate vertices of a sLut.
// This does NOT change those vertices in the output
// not specified in the flags byte.

KLBDC KOLIBA_SLUT * KOLIBA_InterpolateSlutVertices(KOLIBA_SLUT * output, const KOLIBA_SLUT * const input1, const KOLIBA_SLUT * const input2, double factor, unsigned char flags) {
	unsigned int i;
	KOLIBA_VERTEX *vo, *v1, *v2;

	if ((output == NULL) || (input1 == NULL) || (input2 == NULL)) return NULL;

	for (i = 0, vo = (KOLIBA_VERTEX *)output, v1 = (KOLIBA_VERTEX *)input1, v2 = (KOLIBA_VERTEX *)input2; i < 8; i++, vo++, v1++, v2++, flags >>= 1) {
		if (flags & 1) {
			KOLIBA_Interpolate((double *)vo, (double *)v1, factor, (double *)v2, 3);
		}
	}
	return output;
}
