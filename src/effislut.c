/*

	effislut.c

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

#ifndef	KOLIBCALLS
#define	KOLIBCALLS
#endif

#include "koliba.h"
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

/* Apply efficacy to a sLut.

We observe that in an identity sLut
one half of the members are 0.0,
another half 1.0.

	{0.0, 0.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 1.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 1.0}

The ones are at these positions:

5, 7, 10, 11, 12, 15, 17, 18, 19, 21, 22, 23

Given an efficacy, we need to multiply all members
of the sLut by it, then add (1.0-efficacy) to the
half with a 1.0 in the identity sLut.

Rather than check for twelve indices, we can define
an integer with bits 5, 7, 10, etc set, then shift
the bits out. This is much easier to do in assembly
language than in C, though it is not hard in C,
either, just slightly more so.

The reason it is easier in assembly language is
that it allows us to shift the integer right (or
left if we wanted to go backwards) and check the
carry flag, something C cannot do.
*/

#define	ONES	((1<<5)|(1<<7)|(7<<10)|(1<<15)|(7<<17)|(7<<21))

KLBDC KOLIBA_SLUT * KOLIBA_SlutEfficacy(KOLIBA_SLUT *output, const KOLIBA_SLUT * const input, double efficacy) {
	KOLIBA_SLUT s;
	double d;
	double *dinp, *ds;
	int i;
	unsigned int ones;

	if ((output == NULL) || (input == NULL)) return NULL;

	d			 = 1.0 - efficacy;

	for (i = 24, ones = ONES, dinp = (double *)input, ds = (double *)&s; i > 0; i--, ones = ones >> 1, dinp++, ds++)
		if (ones & 1) *ds = d +  efficacy * (*dinp);
		else *ds = efficacy * (*dinp);

	return memcpy(output, &s, sizeof(KOLIBA_FLUT));
}

