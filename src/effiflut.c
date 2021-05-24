/*

	effiflut.c

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

#ifndef	KOLIBCALLS
#define	KOLIBCALLS
#endif

#include "koliba.h"
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Apply efficacy to a fLut.
//
// Instead of calling KOLIBA_Interpolate and passing it KOLIBA_FlutIdentity,
// simplify it keeping in mind that red.r, green.g, and blue.b in KOLIBA_FlutIdentity
// equal 1.0, and everything else in it equals 0.0.
KLBDC KOLIBA_FLUT * KOLIBA_FlutEfficacy(KOLIBA_FLUT * output, const KOLIBA_FLUT * const input, double efficacy) {
	KOLIBA_FLUT f;
	double d;
	double *dinp, *df;
	int i;

	if ((output == NULL) || (input == NULL)) return NULL;

	for (i = 24, dinp = (double *)input, df = (double *)&f; i > 0; i--, dinp++, df++)
		*df 	 = efficacy * (*dinp);

	d			 = 1.0 - efficacy;

	f.red.r		+= d;
	f.green.g	+= d;
	f.blue.b	+= d;

	return memcpy(output, &f, sizeof(KOLIBA_FLUT));
}
