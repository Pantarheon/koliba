/*

	vert2slt.c

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

// Copy a KOLIBA_VERTICES structure into a
// KOLIBA_SLUT structure.
KLBDC KOLIBA_SLUT *KOLIBA_VerticesToSlut(KOLIBA_SLUT *sLut, KOLIBA_VERTICES * const v) {
	if ((v == NULL) || (sLut == NULL)) return NULL;

	sLut->black.r   = v->black->r;
	sLut->black.g   = v->black->g;
	sLut->black.b   = v->black->b;
	sLut->blue.r    = v->blue->r;
	sLut->blue.g    = v->blue->g;
	sLut->blue.b    = v->blue->b;
	sLut->green.r   = v->green->r;
	sLut->green.g   = v->green->g;
	sLut->green.b   = v->green->b;
	sLut->cyan.r    = v->cyan->r;
	sLut->cyan.g    = v->cyan->g;
	sLut->cyan.b    = v->cyan->b;
	sLut->red.r     = v->red->r;
	sLut->red.g     = v->red->g;
	sLut->red.b     = v->red->b;
	sLut->magenta.r = v->magenta->r;
	sLut->magenta.g = v->magenta->g;
	sLut->magenta.b = v->magenta->b;
	sLut->yellow.r  = v->yellow->r;
	sLut->yellow.g  = v->yellow->g;
	sLut->yellow.b  = v->yellow->b;
	sLut->white.r   = v->white->r;
	sLut->white.g   = v->white->g;
	sLut->white.b   = v->white->b;

	return sLut;
}

