/*

	fixdbl.c

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

#ifdef	_WIN32
typedef unsigned long long	BIGINT;
#else
#include	<stdint.h>
typedef	uint64_t	BIGINT;
#endif


// Make sure doubles are in the proper format
// (i.e., LSB first, MSB first, whatever).
//
// We receive a pointer to an MSB first double,
// and a count of doubles.

// This code is generic, so it will work on any system
// regardless of its endianness.
//
// But that makes it not optimized. It is, therefore,
// best to use an assembly language version, which can
// be tailored to whatever architecture.

KLBDC double * KOLIBA_FixDoubles(double *d, unsigned int n) {
#ifndef	BIGENDIAN
	unsigned int i;
	double *dptr;
	unsigned char *ptr;
	union {
		double d;
		BIGINT u;
	} du;

	if ((n == 0) || (d == NULL)) return NULL;

	for (i = 0, dptr = d; i < n; i++, dptr++) {
		ptr = (unsigned char *)dptr;
		du.u = (((ptr[0] & 0xFF) << (8*3)) |
				((ptr[1] & 0xFF) << (8*2)) |
				((ptr[2] & 0xFF) << 8) |
				(ptr[3] & 0xFF));
		du.u = (du.u << 32) |
				(((ptr[4] & 0xFF) << (8*3)) |
				((ptr[5] & 0xFF) << (8*2)) |
				((ptr[6] & 0xFF) << 8) |
				(ptr[7] & 0xFF));
		*dptr = du.d;
	}

#endif	// !defined(BIGENDIAN)
	return d;
}

KLBDC double * KOLIBA_NetDoubles(double *d, unsigned int n) {
#ifndef	BIGENDIAN
	unsigned int i;
	int j;
	double *dptr;
	unsigned char *ptr;
	union {
		double d;
		BIGINT u;
	} du;

	if ((n == 0) || (d == NULL)) return NULL;

	for (i = 0, dptr = d; i < n; i++, dptr++) {
		du.d = *dptr;
		for (j = 7, ptr = (unsigned char *)dptr; j >= 0; j--, du.u >>= 8) {
			ptr[j] = du.u & 0xFF;			;
		}
	}

#endif	// !defined(BIGENDIAN)
	return d;
}
