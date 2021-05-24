/*

	frange.c

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

// Change the extent of the farba.
KLBDC KOLIBA_VERTICES * KOLIBA_FarbaRange(KOLIBA_VERTICES * const vert, const KOLIBA_RGB * const from, const KOLIBA_RGB * const to) {
	register KOLIBA_RGB start, finish;

	if (vert != NULL) {
		if (from != NULL) {
			start.r = from->r;
			start.g = from->g;
			start.b = from->b;
		}
		else {
			start.r = 0.0;
			start.g = 0.0;
			start.b = 0.0;
		}

		if (to != NULL) {
			finish.r = to->r;
			finish.g = to->g;
			finish.b = to->b;
		}
		else {
			finish.r = 1.0;
			finish.g = 1.0;
			finish.b = 1.0;
		}

		vert->blue->r = start.r;
		vert->blue->g = start.g;
		vert->blue->b = finish.b;

		vert->green->r = start.r;
		vert->green->g = finish.g;
		vert->green->b = start.b;

		vert->cyan->r = start.r;
		vert->cyan->g = finish.g;
		vert->cyan->b = finish.b;

		vert->red->r = finish.r;
		vert->red->g = start.g;
		vert->red->b = start.b;

		vert->magenta->r = finish.r;
		vert->magenta->g = start.g;
		vert->magenta->b = finish.b;

		vert->yellow->r = finish.r;
		vert->yellow->g = finish.g;
		vert->yellow->b = start.b;
	}

	return vert;
}
