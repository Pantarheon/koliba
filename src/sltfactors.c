/*

	sltfactors.c

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

// Precalculate sLut factors, store them in KOLIBA_FLUT.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertSlutToFlut(KOLIBA_FLUT *f, const KOLIBA_VERTICES * const s) {
	KOLIBA_FLUT sf;

	if ((f == NULL) || (s == NULL)) return NULL;

	sf.black.r		= s->black->r;
	sf.black.g		= s->black->g;
	sf.black.b		= s->black->b;

	sf.red.r		= s->red->r - s->black->r;
	sf.red.g		= s->red->g - s->black->g;
	sf.red.b		= s->red->b - s->black->b;

	sf.green.r		= s->green->r - s->black->r;
	sf.green.g		= s->green->g - s->black->g;
	sf.green.b		= s->green->b - s->black->b;

	sf.blue.r		= s->blue->r - s->black->r;
	sf.blue.g		= s->blue->g - s->black->g;
	sf.blue.b		= s->blue->b - s->black->b;

	sf.yellow.r		= s->black->r + s->yellow->r - s->green->r - s->red->r;
	sf.yellow.g		= s->black->g + s->yellow->g - s->green->g - s->red->g;
	sf.yellow.b		= s->black->b + s->yellow->b - s->green->b - s->red->b;

	sf.magenta.r	= s->black->r + s->magenta->r - s->blue->r - s->red->r;
	sf.magenta.g	= s->black->g + s->magenta->g - s->blue->g - s->red->g;
	sf.magenta.b	= s->black->b + s->magenta->b - s->blue->b - s->red->b;

	sf.cyan.r		= s->black->r + s->cyan->r - s->blue->r - s->green->r;
	sf.cyan.g		= s->black->g + s->cyan->g - s->blue->g - s->green->g;
	sf.cyan.b		= s->black->b + s->cyan->b - s->blue->b - s->green->b;

	sf.white.r		= s->blue->r + s->green->r + s->red->r + s->white->r - s->black->r - s->cyan->r - s->magenta->r - s->yellow->r;
	sf.white.g		= s->blue->g + s->green->g + s->red->g + s->white->g - s->black->g - s->cyan->g - s->magenta->g - s->yellow->g;
	sf.white.b		= s->blue->b + s->green->b + s->red->b + s->white->b - s->black->b - s->cyan->b - s->magenta->b - s->yellow->b;

	return memcpy(f, KOLIBA_FixFlut(&sf), sizeof(KOLIBA_FLUT));
}



