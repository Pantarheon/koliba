/*

	externxyz.c

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

// Apply a chain of n fLuts to an XYZ vector,
// process the result by an external function,
// then apply a second chain of m fLuts to
// whatever the external changes the result to.
//
// The FFLUT array must be of n+m size, with the
// first n fLuts applied before calling the
// external function, and the next m fLuts
// applied afterwards.
//
// The external function accepts the XYZ vector (which
// is both its input and its output), and a pointer
// any number/type of parameters it needs.
KLBDC KOLIBA_XYZ * KOLIBA_ExternalXyz(KOLIBA_XYZ * xyzout, const KOLIBA_XYZ * const xyzin, const KOLIBA_FFLUT * const fLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, const void * const params) {
	KOLIBA_XYZ temp;

	if ((xyzin == NULL) || (xyzout == NULL)) return NULL;
	else if (ext == NULL) return KOLIBA_PolyXyz(xyzout, xyzin, fLut, n+m);
	else {
		(ext)(KOLIBA_PolyXyz(&temp, xyzin, fLut, n), params);
		return KOLIBA_PolyXyz(xyzout,&temp,&(fLut[n]), m);
	}

/*
	So, why not just do this?

	return KOLIBA_PolyXyz(xyzout,(*ext)(KOLIBA_PolyXyz(&temp, xyzin, fLut, n), params),&(fLut[n]), m);

	It MIGHT compile to a slightly more efficient machine code.
	Or not.

	BUT . . . It would be vulnerable to third party attacks
	(or just third party bugs). The (ext)(...) is SUPPOSED
	to return &temp, just as the PolyXyz inside it does.

	Alas, since anyone can use this library and pass any
	(xyz)() to it, a malicious attacker (or a tired programmer)
	could return some weird pointer and cause a crash, and
	who knows what else.
*/
}

