/*

	windxyz.c

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

// Apply Xyz based on a 16-bit-indexed fLut array,
// with a non-indexed flags array.
//
// In other words, we use it when the above flags
// are KOLIBA_USEFLWINDEX | KOLIBA_NOFFLINDEX.
//
// To be able to insert it in a function array,
// we have one extra argument that we do not use.
//
// We are expecting xyzout and xyzin to be pointing
// at valid addresses, and are not asserting it
// because this function can easily be called millions
// of times in a row, and we prefer not to waste time.
KLBDC KOLIBA_XYZ * KOLIBA_ShortIndexedXyz(KOLIBA_XYZ *xyzout, const KOLIBA_XYZ * const xyzin, const double * const base, const KOLIBA_FLAGS * const flags, const unsigned int dim[3], const void * const flwindex, const void * const v) {
	KOLIBA_FLUT fLut;
	KOLIBA_XYZ xyz;
	unsigned int index;
	register KOLIBA_FLWINDEX *flindex = (KOLIBA_FLWINDEX *)flwindex;

	index = KOLIBA_GetFindex(&xyz, xyzin, dim, NULL);

	return KOLIBA_ApplyXyz(xyzout, &xyz, KOLIBA_ConvertFlwindexToFlut(&fLut, flindex + index, base), flags[index]);
}

