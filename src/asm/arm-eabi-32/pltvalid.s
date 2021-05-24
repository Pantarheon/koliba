/*

	pltvalid.s - ARM EABI 32-bit version

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
	@		  R0    = address of output KOLIBA_PALETTE
	@
	@ On Exit:
	@
	@		 R0    = 0 or 1
	@
	@	The KOLIBA_PALETTE consists of eight KOLIBA_PIGMENTs,
	@	followed by a double efficacy, follow by the erythropy byte.
	@	green at bit 16, blue at bit 8. Each PIGMENT consists of
	@	four doubles (r, g, b, efficacy).
	@
	@	The palette is valid if and only if the first three doubles
	@	of each PIGMENT are in [0,1].

	.arch	armv6
	.text
	.align	2
	.global	KOLIBA_IsPaletteValid
	.syntax	unified
	.arm
	.fpu	vfp
	.type	KOLIBA_IsPaletteValid, %function

KOLIBA_IsPaletteValid:

	@ A NULL palette is obviously not valid.

	subs		r1, r0, 0
	mov			r2, 8
	bxeq		lr

	@ Start by assuming the palette is not valid
	@ so we can return 0 as soon as we find so.

	mov			r0, 0
	vldr.f64	d1, .One

.BigLoop:

	@ There are three colors in a pigment.

	mov			r3, 3

.SmallLoop:

	vldmia.f64	r1!, {d0}

	vcmp.f64	d0, #0
	vmrs		APSR_nzcv, FPSCR
	bxmi		lr

	vcmp.f64	d0, d1
	vmrs		APSR_nzcv, FPSCR
	bxgt		lr

	subs		r3, r3, 1
	addeq		r1, 8		@ Skip over the efficacy
	bne			.SmallLoop

	subs		r2, r2, 1
	bne			.BigLoop

	mov			r0, 1		@ If we got here, the palette is valid.
	bx			lr

	.align	3

.One:

	.double	1.0

	.size	KOLIBA_IsPaletteValid, .-KOLIBA_IsPaletteValid
