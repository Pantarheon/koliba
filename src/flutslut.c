/*

	flutslut.c

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

// Convert fLut factors, to KOLIBA_SLUT.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertFlutToSlut(KOLIBA_SLUT *sLut, const KOLIBA_FLUT * const f) {
	KOLIBA_SLUT sl;

	if ((f == NULL) || (sLut == NULL)) return NULL;

	sl.black.r		= f->black.r;
	sl.black.g		= f->black.g;
	sl.black.b		= f->black.b;

	sl.blue.r		= f->black.r + f->blue.r;
	sl.blue.g		= f->black.g + f->blue.g;
	sl.blue.b		= f->black.b + f->blue.b;

	sl.green.r		= f->black.r + f->green.r;
	sl.green.g		= f->black.g + f->green.g;
	sl.green.b		= f->black.b + f->green.b;

	sl.cyan.r		= sl.green.r + f->blue.r + f->cyan.r;
	sl.cyan.g		= sl.green.g + f->blue.g + f->cyan.g;
	sl.cyan.b		= sl.green.b + f->blue.b + f->cyan.b;

	sl.red.r		= f->black.r + f->red.r;
	sl.red.g		= f->black.g + f->red.g;
	sl.red.b		= f->black.b + f->red.b;

	sl.magenta.r	= sl.red.r + f->blue.r + f->magenta.r;
	sl.magenta.g	= sl.red.g + f->blue.g + f->magenta.g;
	sl.magenta.b	= sl.red.b + f->blue.b + f->magenta.b;

	sl.yellow.r		= sl.red.r + f->green.r + f->yellow.r;
	sl.yellow.g		= sl.red.g + f->green.g + f->yellow.g;
	sl.yellow.b		= sl.red.b + f->green.b + f->yellow.b;

	sl.white.r		= sl.yellow.r + f->blue.r + f->cyan.r + f->magenta.r + f->white.r;
	sl.white.g		= sl.yellow.g + f->blue.g + f->cyan.g + f->magenta.g + f->white.g;
	sl.white.b		= sl.yellow.b + f->blue.b + f->cyan.b + f->magenta.b + f->white.b;

	return (KOLIBA_SLUT *)memcpy(sLut, KOLIBA_FixSlut((KOLIBA_SLUT *)&sl), sizeof(KOLIBA_SLUT));
}



