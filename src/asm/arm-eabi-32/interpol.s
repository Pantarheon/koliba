/*

	interpol.s - ARM EABI 32-bit version

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

	@ On Input:
	@
	@		R0 = address of output
	@		R1 = address of (const) input
	@		R2 = address of (const) modifier
	@		R3 = number of doubles in each of the above
	@		D0 = interpolation rate

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Interpolate
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Interpolate, %function

/*
	This is a linear interpolation, so

		output = input * rate + modifier * (1 - rate)

	That is the same mathematically as,

		output = modifier + rate * (input - modifier)

	But that is simpler to compute as it gets rid of one
	multiplication.
*/

KOLIBA_Interpolate:

	cmp			r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	mov			r12, r0

.LLoop:

	subs		r3, r3, #1
	bxmi		lr
	vldmia.64	r2!, {d7}		@ D7  = modifier
	vldmia.64	r1!, {d6}		@ D6  = input
	vsub.f64	d6, d6, d7		@ D6  = input - modifier
	vmla.f64	d7, d6, d0		@ D7 += rate * D6
	vstmia.64	r12!, {d7}
	b			.LLoop

	.size	KOLIBA_Interpolate, .-KOLIBA_Interpolate
