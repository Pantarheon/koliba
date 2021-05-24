/*

	findex.c

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

// Calculate the index of FLINDEX or FLBINDEX from
// the input x, y, z and the 3D FLUT dimensions.
// Also, scale and shift x, y, z accordingly.
// Optionally, write the i, j, k indices to the
// ind[3] array (which may be NULL if we do not
// want the indices.)
KLBDC signed int KOLIBA_GetFindex(KOLIBA_XYZ * xout, const KOLIBA_XYZ * const xin, const unsigned int dim[3], unsigned int ind[3]) {
	register double x, y, z;
	unsigned int X, Y, Z;
	register int retval;

	if ((xin == NULL) || (xout == NULL)) return -3;

	x = xin->x;
	y = xin->y;
	z = xin->z;

	if (dim == NULL) retval = -2;
	else if ((dim[0] == 0) || (dim[1] == 0) || (dim[2] == 0)) {
		if (ind != NULL) {	// Return values other functions will reject.
			ind[0] = dim[0];
			ind[1] = dim[1];
			ind[2] = dim[2];
		}
		retval = -1;
	}
	else {
		x *= (double)dim[0];
		y *= (double)dim[1];
		z *= (double)dim[2];

		if (x < 1.0) X = 0;
		else {
			X = dim[0] - 1;
			if (x < (double)X) X = (unsigned int)x;
			x -= (double)X;
		}

		if (y < 1.0) Y = 0;
		else {
			Y = dim[1] - 1;
			if (y < (double)Y) Y = (unsigned int)y;
			y -= (double)Y;
		}

		if (z < 1.0) Z = 0;
		else {
			Z = dim[2] - 1;
			if (z < (double)Z) Z = (unsigned int)z;
			z -= (double)Z;
		}

		if (ind != NULL) {
			ind[0] = X;
			ind[1] = Y;
			ind[2] = Z;
		}

		retval = (int)(((X * dim[1]) + Y) * dim[2] + Z);
	}

	xout->x = x;
	xout->y = y;
	xout->z = z;

	return retval;
}
