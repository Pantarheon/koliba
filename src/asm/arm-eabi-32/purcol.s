/*

	purcol.s - ARM EABI 32-bit version

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

	@ On Entry:
	@
	@		R0 = address of output RGB
	@		R1 = address of (const) input RGB
	@		R2 = address of (const) KOLIBA_RGB impurities
	@
	@ On Exit:
	@
	@		R0 = address output RGB or NULL
	@
	@	Unless the input RGB is a shade of gray, we convert
	@	it to a pure color (i.e., give it 100% saturation)
	@	by replacing the lowest-value channel (or two) with 0.0.

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu vfp

	.align	2
	.global	KOLIBA_PureColor
	.type	KOLIBA_PureColor, %function

KOLIBA_PureColor:

	@ If the input or the output is NULL, we have nothing to work with.
	@ The impurities, however, are optional.

	cmp				r1, 0
	moveq			r0, 0
	vldmiane.f64	r1, {d0-d2}
	cmpne			r0, 0
	bxeq			lr

	@ If all three channels are equal, we have a shade of gray,
	@ so just copy it to the output and return. No need to apply
	@ the impurities, either, as the result would be the same.

	vcmp.f64		d0, d1
	vmrs			APSR_nzcv, FPSCR
	blt				.LGreen
	vcmpeq.f64		d1, d2
	vmrseq			APSR_nzcv, FPSCR
	vstmiaeq.f64	r0, {d0-d2}
	bxeq			lr

	@ At least one channel is different from the other two.
	@ Find the one or two channel(s).

	@ Is red (one of) the highest? We know it is not less than
	@ green, so no need to check that. In other words, red is
	@ either greater than or equal to green. In either case,
	@ if red is lesser than blue, then blue alone is max. Else,
	@ red is max, whether alone or along with either green or
	@ blue but not both because we know all three are not equal.

	vcmp.f64		d0, d2
	vmrs			APSR_nzcv, FPSCR
	blt				.LBlue

	@ Yes, red is max. Set the min one or two to 0. Strictly
	@ speaking, we could tell that if blue is equal to red here,
	@ green is the sole min. Alas, that would add at least an
	@ extra two lines of code, so we are just going to compare
	@ blue and green to KISS.
	
	vcmp.f64		d1, d2
	vmrs			APSR_nzcv, FPSCR

	@ If they are equal, set both to 0.
	@ Else, if D1 > D2, set D2 to 0.
	@ Else, set D1 to 0.
	@ So, we set D1 to 0 if D1 <= D2,
	@ and we set D2 to 0 if D1 >= D2.

	vsuble.f64		d1, d1, d1

.LD2:

	vsubge.f64		d2, d2, d2

.LDone:

	@ If R2 is not NULL, apply impurities.
	@ Otherwise just return the results.

	cmp				r2, 0
	vldmiane.f64	r1, {d3-d5}
	vldmiane.f64	r2!, {d6-d7}

	vsubne.f64		d3, d3, d0
	vsubne.f64		d4, d4, d1
	vsubne.f64		d5, d5, d2
	vmlane.f64		d0, d3, d6
	vldrne.f64		d6, [r2]
	vmlane.f64		d1, d4, d7
	vmlane.f64		d2, d5, d6
	vstmia.f64		r0, {d0-d2}
	bx				lr

.LGreen:

	@ Is green max? We already know it is greater than red,
	@ so no need to check that again. But that still does not
	@ necessarily mean green is max, only that red is not.

	vcmp.f64		d1, d2
	vmrs			APSR_nzcv, FPSCR
	blt				.LBlue

	@ Yes, green is max. Set the min one or two to 0.
	
	vcmp.f64		d0, d2
	vmrs			APSR_nzcv, FPSCR

	@ If they are equal, set both to 0.
	@ Else, if D0 > D2, set D2 to 0.
	@ Else, set D0 to 0.
	@ So, we set D0 to 0 if D0 <= D2,
	@ and we set D2 to 0 if D0 >= D2.

	vsuble.f64		d0, d0, d0
	b				.LD2

.LBlue:

	@ Not only is blue max, it is greater than both other channels.
	@ We, however, still need to determine which of the other two
	@ is min. May or may not be both, but at least one of them is.

	vcmp.f64		d0, d1
	vmrs			APSR_nzcv, FPSCR
	vsuble.f64		d0, d0, d0
	vsubge.f64		d1, d1, d1
	b				.LDone

	.size	KOLIBA_PureColor, .-KOLIBA_PureColor
