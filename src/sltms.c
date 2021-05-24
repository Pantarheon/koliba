/*

	sltms.c

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

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

KLBDC KOLIBA_SLUT * KOLIBA_ConvertMatrixToSlut(KOLIBA_SLUT * sLut, const KOLIBA_MATRIX * const mat) {
	if ((sLut == NULL) || (mat == NULL)) return NULL;

	sLut->black.r = mat->red.o;
	sLut->black.g = mat->green.o;
	sLut->black.b = mat->blue.o;
	sLut->blue.r = mat->red.b+mat->red.o;
	sLut->blue.g = mat->green.b+mat->green.o;
	sLut->blue.b = mat->blue.b+mat->blue.o;
	sLut->green.r = mat->red.g+mat->red.o;
	sLut->green.g = mat->green.g+mat->green.o;
	sLut->green.b = mat->blue.g+mat->blue.o;
	sLut->cyan.r = mat->red.b+mat->red.g+mat->red.o;
	sLut->cyan.g = mat->green.b+mat->green.g+mat->green.o;
	sLut->cyan.b = mat->blue.b+mat->blue.g+mat->blue.o;
	sLut->red.r = mat->red.r+mat->red.o;
	sLut->red.g = mat->green.r+mat->green.o;
	sLut->red.b = mat->blue.r+mat->blue.o;
	sLut->magenta.r = mat->red.r+mat->red.b+mat->red.o;
	sLut->magenta.g = mat->green.r+mat->green.b+mat->green.o;
	sLut->magenta.b = mat->blue.r+mat->blue.b+mat->blue.o;
	sLut->yellow.r = mat->red.r+mat->red.g+mat->red.o;
	sLut->yellow.g = mat->green.r+mat->green.g+mat->green.o;
	sLut->yellow.b = mat->blue.r+mat->blue.g+mat->blue.o;
	sLut->white.r = mat->red.b+mat->red.g+mat->red.r+mat->red.o;
	sLut->white.g = mat->green.b+mat->green.g+mat->green.r+mat->green.o;
	sLut->white.b = mat->blue.b+mat->blue.g+mat->blue.r+mat->blue.o;

	return sLut;
}
