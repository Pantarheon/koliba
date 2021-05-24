/*

	chrmreset.c

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

// Reset a chromatic matrix to default contents, plus set
// the model. If no model is specified, do not change it
// and only reset the rest.

KLBDC KOLIBA_CHROMAT * KOLIBA_ResetChromaticMatrix(KOLIBA_CHROMAT * chrm, const KOLIBA_RGB * const model) {
	if (chrm != NULL) {
		if ((model != NULL) && (model != &chrm->model)) KOLIBA_SetChromaModel(&chrm->model, model);
		chrm->chroma.angle      = 0.0;
		chrm->chroma.magnitude  = 1.0;
		chrm->chroma.saturation = 1.0;
		chrm->chroma.black      = 0.0;
		chrm->chroma.white      = 1.0;
	}

	return chrm;
}

// Set the model inside a chromatic matrix without changing
// the chromatic matrix itself.

KLBDC KOLIBA_CHROMAT * KOLIBA_SetChromaticMatrixModel(KOLIBA_CHROMAT * chrm, const KOLIBA_RGB * const model) {
	if (chrm != NULL) KOLIBA_SetChromaModel(&chrm->model, model);
	return chrm;
}
