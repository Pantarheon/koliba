/*

	writem3x4.c

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

// Write a matrix to an open .m3x4 file. It needs to be open
// for writing binary data. It remains open upon return, so
// the caller needs to close it. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WriteMatrixToOpenFile(const KOLIBA_MATRIX *m3x4, FILE *f) {
	KOLIBA_MATRIX2 mat;

	if ((m3x4 == NULL) || (f == NULL)) return -2;
	memcpy(&mat.mat, m3x4, sizeof(KOLIBA_MATRIX));
	mat.checksum = KOLIBA_CalcSum((double *)&mat.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
	KOLIBA_NetDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2) / sizeof(double));
	if (fwrite(&KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f) != SLTCFILEHEADERBYTES) return 1;
	return (fwrite(&mat, 1, sizeof(KOLIBA_MATRIX2), f) == sizeof(KOLIBA_MATRIX2)) ? 0 : 2;
}

// Write a matrix to a named file. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WriteMatrixToNamedFile(const KOLIBA_MATRIX *mat, const char *fname) {
	FILE *f;
	int retval;

	if ((mat == NULL) || (fname == NULL) || ((f = fopen(fname, "wb")) == NULL)) return -1;
	retval = KOLIBA_WriteMatrixToOpenFile(mat, f);
	fclose(f);
	return retval;
}
