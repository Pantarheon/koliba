/*

	fflut.c

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

// Find the fastest way to apply a fLut to a pixel.
// This simply returns an integer which flags non-zero fLut
// vertices, so they can be skipped in the computation.
//
// It also flags the lowest three bits regardless of the
// corresponding fLut values because of their special
// nature in the FLUT-interpolating algorithm.
//
// That is why the starting value of "method" in the for
// loop is 7 (= 1 | 2 | 4), the starting pointer is &(fLut->red),
// rather than just fLut, and the orring is by 8 (= 1 << 3) << i,
// rather than 1 << i.
//
// Keep all that in mind if you are porting the Koliba library
// to a different environment or are designing a Koliba chip.

KLBDC KOLIBA_FLAGS KOLIBA_FlutFlags(const KOLIBA_FLUT * const fLut) {
	KOLIBA_FLAGS method;
	unsigned int i;
	double *ptr;

	if (fLut == NULL) return KOLIBA_AllFlutFlags;

	for (i = 0, method = 7, ptr = (double *)&(fLut->red); i < 3*7; i++, ptr++) {
		if (*ptr != 0.0) method |= (8 << i);
	}

	return (method);
}