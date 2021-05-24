/*

	readm3x4.c

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

static KOLIBA_MATRIX * invalid(KOLIBA_MATRIX *m3x4) {
	memcpy(m3x4, &KOLIBA_IdentityMatrix, sizeof(KOLIBA_MATRIX));
	return NULL;
}

// Read a matrix from an open .m3x4 file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns matrix on success, NULL
// on failure. If, however, m3x4 is not NULL, its contents
// will be filled with the identity matrix on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromOpenFile(KOLIBA_MATRIX *m3x4, FILE *f) {
	KOLIBA_MATRIX2 mat;
	unsigned char header[SLTCFILEHEADERBYTES];

	if (m3x4 == NULL) return NULL;
	if (f == NULL) return invalid(m3x4);

	if (fread(header, 1, SLTCFILEHEADERBYTES, f) != SLTCFILEHEADERBYTES) return invalid(m3x4);
	if (memcmp(header, &KOLIBA_m3x4Header, SLTCFILEHEADERBYTES) != 0) return invalid(m3x4);
	if (fread(&mat, 1, sizeof(KOLIBA_MATRIX2), f) != sizeof(KOLIBA_MATRIX2)) return invalid(m3x4);
	KOLIBA_FixDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2) / sizeof(double));
	if (KOLIBA_CheckMat(&mat.mat, mat.checksum) == 0) return invalid(m3x4);
	return memcpy(m3x4, &mat.mat, sizeof(KOLIBA_MATRIX));
}

// Read a mmatrix from a named file. Returns mmatrix on success,
// NULL on failure. If, however, m3x4 is not NULL, its
// contents will be filled with the identity matrix on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromNamedFile(KOLIBA_MATRIX *m3x4, char *fname) {
	FILE *f;
	KOLIBA_MATRIX *retval;

	if (fname == NULL) return (m3x4 == NULL) ? NULL : invalid(m3x4);
	retval = KOLIBA_ReadMatrixFromOpenFile(m3x4, f = fopen(fname, "rb"));
	if (f != NULL) fclose(f);
	return retval;
}
