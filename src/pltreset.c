/*

	pltreset.c

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

// Reset a palette to default contents

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPalette(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(plt, &KOLIBA_IdentityPalette, sizeof(KOLIBA_PALETTE));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteBlack(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->black, &KOLIBA_IdentityPalette.black, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteWhite(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->white, &KOLIBA_IdentityPalette.white, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteRed(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->red, &KOLIBA_IdentityPalette.red, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteGreen(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->green, &KOLIBA_IdentityPalette.green, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteBlue(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->blue, &KOLIBA_IdentityPalette.blue, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteCyan(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->cyan, &KOLIBA_IdentityPalette.cyan, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteMagenta(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->magenta, &KOLIBA_IdentityPalette.magenta, sizeof(KOLIBA_PIGMENT));
	return plt;
}

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteYellow(KOLIBA_PALETTE *plt) {
	if (plt != NULL) memcpy(&plt->yellow, &KOLIBA_IdentityPalette.yellow, sizeof(KOLIBA_PIGMENT));
	return plt;
}
