/*

	flutter.c

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

KLBHID extern const double KOLIBA_Zeroes[24];

// The flutter blend. This operation is vaguely analogous
// to matrix multiplication, but that is not truly defined
// for two 8x3 matrices (which fLuts are).
//
// Nevertheless, it can produce a new effect. So, why not?

KLBDC KOLIBA_FLUT * KOLIBA_Flutter(KOLIBA_FLUT *output, const KOLIBA_FLUT * const modificand, const KOLIBA_FLUT * const modifier) {
	KOLIBA_FLUT fLut;
	double	D, E, F, G, H, I, J, K, L;

	if ((output == NULL) || (modifier == NULL) || (modificand == NULL)) return NULL;

#define	A	(modifier->black.r)
#define	B	(modifier->black.g)
#define	C	(modifier->black.b)

	D = modifier->red.r;
	E = modifier->red.g;
	F = modifier->red.b;

	G = modifier->green.r;
	H = modifier->green.g;
	I = modifier->green.b;

	J = modifier->blue.r;
	K = modifier->blue.g;
	L = modifier->blue.b;

#define	M	(modifier->yellow.r)
#define	N	(modifier->yellow.g)
#define	O	(modifier->yellow.b)

#define	P	(modifier->magenta.r)
#define	Q	(modifier->magenta.g)
#define	R	(modifier->magenta.b)

#define	S	(modifier->cyan.r)
#define	T	(modifier->cyan.g)
#define	U	(modifier->cyan.b)

#define	V	(modifier->white.r)
#define	W	(modifier->white.g)
#define	X	(modifier->white.b)

#define	Z	(*KOLIBA_Zeroes)

#define	ma	(modificand->black.r)
#define	mb	(modificand->black.g)
#define	mc	(modificand->black.b)

#define	md	(modificand->red.r)
#define	me	(modificand->red.g)
#define	mf	(modificand->red.b)

#define	mg	(modificand->green.r)
#define	mh	(modificand->green.g)
#define	mi	(modificand->green.b)

#define	mj	(modificand->blue.r)
#define	mk	(modificand->blue.g)
#define	ml	(modificand->blue.b)

#define	mm	(modificand->yellow.r)
#define	mn	(modificand->yellow.g)
#define	mo	(modificand->yellow.b)

#define	mp	(modificand->magenta.r)
#define	mq	(modificand->magenta.g)
#define	mr	(modificand->magenta.b)

#define	ms	(modificand->cyan.r)
#define	mt	(modificand->cyan.g)
#define	mu	(modificand->cyan.b)

#define	mv	(modificand->white.r)
#define	mw	(modificand->white.g)
#define	mx	(modificand->white.b)

	fLut.black.r	= A + D*ma + G*mb + J*mc;
	fLut.black.g	= B + E*ma + H*mb + K*mc;
	fLut.black.b	= C + F*ma + I*mb + L*mc;

	fLut.red.r		=     D*md + G*me + J*mf;
	fLut.red.g		=     E*md + H*me + K*mf;
	fLut.red.b		=     F*md + I*me + L*mf;

	fLut.green.r	=     D*mg + G*mh + J*mi;
	fLut.green.g	=     E*mg + H*mh + K*mi;
	fLut.green.b	=     F*mg + I*mh + L*mi;

	fLut.blue.r		=     D*mj + G*mk + J*ml;
	fLut.blue.g		=     E*mj + H*mk + K*ml;
	fLut.blue.b		=     F*mj + I*mk + L*ml;

	fLut.yellow.r	= M + D*mm + G*mn + J*mo;
	fLut.yellow.g	= N + E*mm + H*mn + K*mo;
	fLut.yellow.b	= O + F*mm + I*mn + L*mo;

	fLut.magenta.r	= P + D*mp + G*mq + J*mr;
	fLut.magenta.g	= Q + E*mp + H*mq + K*mr;
	fLut.magenta.b	= R + F*mp + I*mq + L*mr;

	fLut.cyan.r		= S + D*ms + G*mt + J*mu;
	fLut.cyan.g		= T + E*ms + H*mt + K*mu;
	fLut.cyan.b		= U + F*ms + I*mt + L*mu;

	fLut.white.r	= V + D*mv + G*mw + J*mx;
	fLut.white.g	= W + E*mv + H*mw + K*mx;
	fLut.white.b	= X + F*mv + I*mw + L*mx;

	return (KOLIBA_FLUT *)memcpy(output, &fLut, sizeof(KOLIBA_FLUT));
}
