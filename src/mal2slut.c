/*

	mal2slut.c

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

// Convert an individual mallet to a sLut. If the mallet is NULL
// or the flags equal to 0, copy iLut to the output. If iLut is NULL,
// the identity sLut is used. If the gray factors structure is NULL,
// Rec. 2020 is used.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertMalletToSlut(KOLIBA_SLUT * sLut, const KOLIBA_SLUT * const iLut, const KOLIBA_MALLET * const mallet, const KOLIBA_RGB * const gray) {
	if (sLut != NULL) {
		KOLIBA_SLUT slut;
		KOLIBA_SLUT temp;

		memcpy(&slut, (iLut == NULL) ? &KOLIBA_IdentitySlut : iLut, sizeof(KOLIBA_SLUT));
		if ((mallet != NULL) && (mallet->flags != '\0')) {
			if (mallet->invert != '\0') KOLIBA_InvertSlutVertices(&slut, mallet->flags);
			if (mallet->flags == KOLIBA_SLUTWHITE)
				KOLIBA_ApplyGainToSlutVertices(&slut, KOLIBA_SLUTWHITE, mallet->gain);
			else
				KOLIBA_ApplyLiftToSlutVertices(&slut,mallet->flags,mallet->lift);
			if (mallet->adjustment != 0.0) {
				KOLIBA_ApplyContrastToSlutVertices(&slut,KOLIBA_CopyColorToSlutVertices(&temp,&mallet->center),mallet->flags,mallet->adjustment);
			}
			KOLIBA_ApplyContrastToSlutVertices(&slut,&KOLIBA_NaturalContrastSlut,mallet->flags,mallet->natcon);
			if (mallet->flags == KOLIBA_SLUTWHITE)
				KOLIBA_ApplyLiftToSlutVertices(&slut,KOLIBA_SLUTWHITE,mallet->lift);
			else
				KOLIBA_ApplyGainToSlutVertices(&slut, mallet->flags, mallet->gain);
			KOLIBA_AddOffsetToSlutVertices(&slut, mallet->flags, mallet->offset);
			KOLIBA_ApplyEfficacyToSlutVertices(&slut,mallet->flags,mallet->efficacy);
			KOLIBA_ApplySaturationToSlutVertices(&slut,(gray==NULL)?&KOLIBA_Rec2020Slut:KOLIBA_ConvertGrayToSlut(&temp,gray),mallet->flags,mallet->saturation);
		}
		memcpy(sLut, &slut, sizeof(KOLIBA_SLUT));
	}

	return sLut;
}
