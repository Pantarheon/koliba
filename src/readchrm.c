/*

	readchrm.c

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

static KOLIBA_CHROMAT * invalid(KOLIBA_CHROMAT *chrm) {
	KOLIBA_ResetChromaticMatrix(chrm, &KOLIBA_Rec2020);
	return NULL;
}

// Read a chromatic matrix from an open .chrm file. It needs
// to be open for reading binary data. It remains open upon
// return, so the caller needs to close it. Returns chromatic
// matrix on success, NULL on failure. If, however, chrm is
// not NULL, its contents will be filled with the identity
// chromatic matrix, using the Rec. 2020 model, on failure.

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChromaticMatrixFromOpenFile(KOLIBA_CHROMAT *chrm, FILE *f) {
	KOLIBA_CHROMAT2 mat;
	unsigned char header[SLTCFILEHEADERBYTES];

	if (chrm == NULL) return NULL;
	if (f == NULL) return invalid(chrm);

	if (fread(header, 1, SLTCFILEHEADERBYTES, f) != SLTCFILEHEADERBYTES) return invalid(chrm);
	if (memcmp(header, &KOLIBA_chrmHeader, SLTCFILEHEADERBYTES) != 0) return invalid(chrm);
	if (fread(&mat, 1, sizeof(KOLIBA_CHROMAT2), f) != sizeof(KOLIBA_CHROMAT2)) return invalid(chrm);
	KOLIBA_FixDoubles((double *)&mat, sizeof(KOLIBA_CHROMAT2) / sizeof(double));
	if (KOLIBA_CheckChrm(&mat.chromat, mat.checksum) == 0) return invalid(chrm);
	return memcpy(chrm, &mat.chromat, sizeof(KOLIBA_CHROMAT));
}

// Read a mmatrix from a named file. Returns chromatic
// matrix on success, NULL on failure. If, however, chrm is
// not NULL, its contents will be filled with the identity
// chromatic matrix, using the Rec. 2020 model, on failure.

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChromaticMatrixFromNamedFile(KOLIBA_CHROMAT *chrm, char *fname) {
	FILE *f;
	KOLIBA_CHROMAT *retval;

	if (fname == NULL) return (chrm == NULL) ? NULL : invalid(chrm);
	retval = KOLIBA_ReadChromaticMatrixFromOpenFile(chrm, f = fopen(fname, "rb"));
	if (f != NULL) fclose(f);
	return retval;
}
