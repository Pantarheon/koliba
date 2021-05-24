/*

	effilut.c

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

// Apply an EFFILUT.

KLBDC KOLIBA_SLUT * KOLIBA_ApplyEfficacies(KOLIBA_SLUT * output, const KOLIBA_SLUT * const input, const KOLIBA_EFFILUT * const efficacies, const KOLIBA_SLUT * const modifier) {
	if ((output == NULL) || (input == NULL) || (efficacies == NULL) || (modifier == NULL)) return NULL;

	else {
		KOLIBA_VERTEX *overtex = (KOLIBA_VERTEX *)output;
		const KOLIBA_VERTEX *ivertex = (KOLIBA_VERTEX *)input;
		const double *effi = (double *)efficacies;
		const KOLIBA_VERTEX *mvertex = (KOLIBA_VERTEX *)modifier;
		unsigned int i;

		for (i = 0; i < sizeof(KOLIBA_SLUT)/sizeof(KOLIBA_VERTEX); i++, overtex++, ivertex++, effi++, mvertex++)
			KOLIBA_Interpolate((double *)overtex, (double *)ivertex, (double)*effi, (double *)mvertex, sizeof(KOLIBA_VERTEX)/sizeof(double));

		return output;
	}
}

