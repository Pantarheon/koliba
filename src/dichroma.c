/*

	dichroma.c

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

// Create a dichromatic matrix, defined by a chroma from
// an angle (in degrees) and its complement. Optionally,
// the matrix may be normalized to preserve the brightness.

KLBDC KOLIBA_MATRIX * KOLIBA_DichromaticMatrix(KOLIBA_MATRIX * mat, const KOLIBA_DICHROMA * const dicr, unsigned int normalize, unsigned int channel) {
	KOLIBA_MATRIX m, m2;
	KOLIBA_CHROMAT chrm;

	if ((mat == NULL) || (dicr == NULL) || (channel > 2)) return NULL;

	KOLIBA_ResetChromaticMatrix(&chrm, &(dicr->chr.model));
	KOLIBA_ChromaticMatrix(&m2, &(dicr->chr));
	chrm.chroma.saturation = 0.0;
	KOLIBA_ChromaticMatrix(&m, &chrm);

	switch (channel) {
	case 0:
		memcpy(&m.red, &m2.red, sizeof(KOLIBA_ROW));
		KOLIBA_NormalizeMatrixRow(&m.red, normalize);
		break;
	case 1:
		memcpy(&m.green, &m2.green, sizeof(KOLIBA_ROW));
		KOLIBA_NormalizeMatrixRow(&m.green, normalize);
		break;
	case 2:
		memcpy(&m.blue, &m2.blue, sizeof(KOLIBA_ROW));
		KOLIBA_NormalizeMatrixRow(&m.blue, normalize);
		break;
	}

	if (dicr->rotation != 0.0) {
		chrm.chroma.saturation = 1.0;
		chrm.chroma.angle      = dicr->rotation;
		KOLIBA_MultiplyMatrices(&m, &m, KOLIBA_ChromaticMatrix(&m2, &chrm));
	}

	return KOLIBA_MatrixEfficacy(mat, &m, dicr->efficacy);
}
