/*

	plut2slt.c

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

// Convert a pLut to a sLut.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertPlutToSlut(KOLIBA_SLUT *slut, const KOLIBA_PLUT * const plut) {
	KOLIBA_SLUT lut;
	double *dptr;
	double d;
	unsigned int i;

	if ((plut == NULL) || (slut == NULL) || (plut->divisor < 1.0)) return NULL;

	memcpy(&lut.black, &plut->black, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.blue, &plut->blue, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.green, &plut->green, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.cyan, &plut->cyan, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.red, &plut->red, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.magenta, &plut->magenta, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.yellow, &plut->yellow, sizeof(KOLIBA_VERTEX));
	memcpy(&lut.white, &plut->white, sizeof(KOLIBA_VERTEX));
	d = plut->divisor;
	if (d != 1.0) {	// No need to check for 0 because we have returned NULL
					// if d < 1.0 already.
		for (i = 0, dptr = (double *)&lut; i < sizeof(KOLIBA_SLUT)/sizeof(double); i++, dptr++)
			*dptr /= d;
	}
	return KOLIBA_SlutEfficacy(slut, &lut, plut->efficacy);
}
