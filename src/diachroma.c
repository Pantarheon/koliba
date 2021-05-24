/*

	diachroma.c

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

// Create a diachromatic matrix, defined by three chromas.

KLBDC KOLIBA_MATRIX * KOLIBA_DiachromaticMatrix(KOLIBA_MATRIX * mat, const KOLIBA_DIACHROMA * const dacr, unsigned int normalize) {
	KOLIBA_MATRIX m, m1, m2;
	KOLIBA_CHROMAT chrm;

	if ((mat == NULL) || (dacr == NULL)) return NULL;

	KOLIBA_ChromaMatrix(&m, &(dacr->chr[0]), &(dacr->model));
	KOLIBA_ChromaMatrix(&m1, &(dacr->chr[1]), &(dacr->model));
	memcpy(&m.green, &m1.green, sizeof(KOLIBA_ROW));

	KOLIBA_ChromaMatrix(&m1, &(dacr->chr[2]), &(dacr->model));
	memcpy(&m.blue, &m1.blue, sizeof(KOLIBA_ROW));

	if (normalize) {
		KOLIBA_NormalizeMatrixRow(&m.red, normalize & NORMALIZE_RED);
		KOLIBA_NormalizeMatrixRow(&m.green, normalize & NORMALIZE_GREEN);
		KOLIBA_NormalizeMatrixRow(&m.blue, normalize & NORMALIZE_BLUE);
	}

	if (dacr->rotation != 0.0) {
		KOLIBA_ResetChromaticMatrix(&chrm, &(dacr->model));
		chrm.chroma.angle      = dacr->rotation;
		KOLIBA_MultiplyMatrices(&m, &m, KOLIBA_ChromaticMatrix(&m1, &chrm));
	}

	return KOLIBA_MatrixEfficacy(mat, &m, dacr->efficacy);
}
