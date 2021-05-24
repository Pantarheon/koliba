/*

	mulsluts.c

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

// "Multiply" sLuts. This operation is only analogous
// to matrix multiplication, but is not as precise.

KLBDC KOLIBA_SLUT * KOLIBA_MultiplySluts(KOLIBA_SLUT * output, const KOLIBA_SLUT * const multiplicand, const KOLIBA_SLUT * const multiplier) {
	KOLIBA_VERTICES v;
	KOLIBA_XYZ *xyzin, *xyzout;
	KOLIBA_SLUT sLut;
	KOLIBA_FLUT fLut;
	unsigned int i;
	KOLIBA_FLAGS flags;

	if ((output == NULL) || (multiplier == NULL) || (multiplicand == NULL)) return NULL;

	flags = KOLIBA_FlutFlags(KOLIBA_ConvertSlutToFlut(&fLut, KOLIBA_SlutToVertices(&v, (KOLIBA_SLUT *)multiplier)));

	for (i = 8, xyzin = (KOLIBA_XYZ *)multiplicand, xyzout = (KOLIBA_XYZ *)&sLut; i!= 0; i--, xyzin++, xyzout++) {
		KOLIBA_ApplyXyz(xyzout, xyzin, &fLut, flags);
	}

	return memcpy(output, &sLut, sizeof(KOLIBA_SLUT));
}
