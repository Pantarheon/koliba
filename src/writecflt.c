/*

	writecflt.c

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

#define	USECLIB
#include "koliba.h"
#include <stdio.h>
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

// Write a Color Filter to an open .cFlt file. It needs to be
// open for writing binary data. It remains open upon return,
// so the caller needs to close it. Returns 0 on success,
// non-0 on failure.

KLBDC int KOLIBA_WriteColorFilterToOpenFile(const KOLIBA_CFLT *cFlt, FILE *f) {
	KOLIBA_CFLT2 Flt;

	if ((cFlt == NULL) || (f == NULL)) return -2;
	memcpy(&Flt.cFlt, cFlt, sizeof(KOLIBA_CFLT));
	Flt.checksum = KOLIBA_CalcSum((double *)&Flt.cFlt, sizeof(KOLIBA_CFLT) / sizeof(double));
	KOLIBA_NetDoubles((double *)&Flt, sizeof(KOLIBA_CFLT2) / sizeof(double));
	if (fwrite(&KOLIBA_cFltHeader, 1, SLTCFILEHEADERBYTES, f) != SLTCFILEHEADERBYTES) return 1;
	return (fwrite(&Flt, 1, sizeof(KOLIBA_CFLT2), f) == sizeof(KOLIBA_CFLT2)) ? 0 : 2;
}

// Write a Color Filter to a named file. Returns 0 on success,
// non-0 on failure.

KLBDC int KOLIBA_WriteColorFilterToNamedFile(const KOLIBA_CFLT *cFlt, const char *fname) {
	FILE *f;
	int retval;

	if ((cFlt == NULL) || (fname == NULL) || ((f = fopen(fname, "wb")) == NULL)) return -1;
	retval = KOLIBA_WriteColorFilterToOpenFile(cFlt, f);
	fclose(f);
	return retval;
}
