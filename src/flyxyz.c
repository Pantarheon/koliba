/*

	flyxyz.c

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

// Apply Xyz based on an on-the-fly fLut array.
// That is, the array is filled as we go.
// We determine whether a specific fLut has to be
// filled by comparing its flags to 0.
// Both, the fLut array and the flag array must
// be in a writable section of the memory.
//
// Hint: If you need to change any parameters
// to the (*KOLIBA_MAKEVERTEX)() function, just
// zero-out the entire flag array to re-apply
// the function with its new parameters.
KLBDC KOLIBA_XYZ * KOLIBA_FlyXyz(KOLIBA_XYZ * xyzout, const KOLIBA_XYZ * const xyzin, KOLIBA_FLUT * fLut, KOLIBA_FLAGS * const flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void * const params) {
	KOLIBA_SLUT sLut;
	KOLIBA_VERTICES vert;
	KOLIBA_XYZ xyz;
	unsigned int ind[3];
	signed int index;

	index = KOLIBA_GetFindex(&xyz, xyzin, dim, ind);

	if (index < 0) {
		xyzout->x = xyzin->x;
		xyzout->y = xyzin->y;
		xyzout->z = xyzin->z;
		return xyzout;
	}

	if (flags[index] == 0) {
		KOLIBA_MakePartialSlut(&sLut, dim, ind, fn, params);
		KOLIBA_SlutToVertices(&vert, &sLut);
		KOLIBA_ConvertSlutToFlut(&(fLut[index]), &vert);
		flags[index] = KOLIBA_FlutFlags(&(fLut[index]));
	}

	return KOLIBA_ApplyXyz(xyzout, &xyz, &(fLut[index]), flags[index]);
}

