/*

	fixslut.c

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

#define	TINY	(1.0/4294967296.0)

// Change very small values to 0
// because certain cube LUT plug-ins
// get confused by very small numbers.

KLBDC KOLIBA_SLUT * KOLIBA_FixSlut(KOLIBA_SLUT *sLut) {
	double *ptr;
	unsigned int i;

	if (sLut == NULL) return NULL;

	for (i = sizeof(KOLIBA_SLUT)/sizeof(double), ptr = (double *)sLut; i != 0 ; i--,ptr++)
		if (fabs(*ptr) <= TINY) *ptr = 0.0;
	return sLut;
}

// Since both KOLIBA_SLUT and KOLIBA_FLUT are really just arrays
// of 24 doubles, it takes identical machine code to fix them.
KLBDC KOLIBA_FLUT * KOLIBA_FixFlut(KOLIBA_FLUT *fLut) {
	return (KOLIBA_FLUT *)KOLIBA_FixSlut((KOLIBA_SLUT *)fLut);
}
