/*

	readslut.c

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

static KOLIBA_SLUT * invalid(KOLIBA_SLUT *sLut) {
	memcpy(sLut, &KOLIBA_IdentitySlut, sizeof(KOLIBA_SLUT));
	return NULL;
}

// Read a sLut from an open .sLut file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns sLut on success, NULL
// on failure. If, however, sLut is not NULL, its contents
// will be filled with the identity sLut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromOpenFile(KOLIBA_SLUT *sLut, FILE *f) {
	KOLIBA_SLUT2 Lut;
	unsigned char header[SLTCFILEHEADERBYTES];

	if (sLut == NULL) return NULL;
	if (f == NULL) return invalid(sLut);

	if (fread(header, 1, SLTCFILEHEADERBYTES, f) != SLTCFILEHEADERBYTES) return invalid(sLut);
	if (memcmp(header, &KOLIBA_sLutHeader, SLTCFILEHEADERBYTES) != 0) return invalid(sLut);
	if (fread(&Lut, 1, sizeof(KOLIBA_SLUT2), f) != sizeof(KOLIBA_SLUT2)) return invalid(sLut);
	KOLIBA_FixDoubles((double *)&Lut, sizeof(KOLIBA_SLUT2) / sizeof(double));
	if (KOLIBA_CheckSlut(&Lut.sLut, Lut.checksum) == 0) return invalid(sLut);
	return memcpy(sLut, &Lut.sLut, sizeof(KOLIBA_SLUT));
}

// Read a sLut from a named file. Returns sLut on success,
// NULL on failure. If, however, sLut is not NULL, its
// contents will be filled with the identity sLut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromNamedFile(KOLIBA_SLUT *sLut, char *fname) {
	FILE *f;
	KOLIBA_SLUT *retval;

	if (fname == NULL) return (sLut == NULL) ? NULL : invalid(sLut);
	retval = KOLIBA_ReadSlutFromOpenFile(sLut, f = fopen(fname, "rb"));
	if (f != NULL) fclose(f);
	return retval;
}
