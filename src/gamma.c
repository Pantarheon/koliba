/*

	gamma.c

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
#include <math.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

#define	prep(p,q)	\
	if ((q) < 0.0) p = (1.0-q);\
	else p = 1.0 / (1.0+q);


KLBDC KOLIBA_XYZ * KOLIBA_PrepareGammaParameters(KOLIBA_XYZ * params, const KOLIBA_XYZ * const q) {
	if ((params == NULL) || (q == NULL)) return NULL;

	prep(params->x, q->x)
	prep(params->y, q->y)
	prep(params->z, q->z)

	return params;
}


// An example function of the KOLIBA_EXTERNAL variety.
// In preparation, you can use KOLIBA_PrepareGammaParameters()
// to make sure all parameters have reasonable values.
//
// Please note that in this (or any KOLIBA_EXTERNAL) function
// *xyz is both the input and the output and that if *params==NULL,
// we are not returning NULL, we are just returning without
// doing anything. Because the show must go on, it is better
// to just return any pixel unchanged than to complain about
// something that we would traditionally view as an error.
KLBDC KOLIBA_XYZ * KOLIBA_Gamma(KOLIBA_XYZ * xyz, const void * const params) {
	if ((xyz != NULL) && (params != NULL)) {
		const KOLIBA_XYZ *p = (KOLIBA_XYZ *)params;

		xyz->x = pow(xyz->x, p->x);
		xyz->y = pow(xyz->y, p->y);
		xyz->z = pow(xyz->z, p->z);
	}

	return xyz;
}

