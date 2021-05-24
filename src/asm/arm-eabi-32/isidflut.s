/*

	isidflut.s - ARM EABI 32-bit version

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
	@		R0 = address of (const) input FLUT
	@
	@ On Exit:
	@
	@		R0 = 1 if FLUT is I, else 0

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	3
	.global	KOLIBA_IsIdentityFlut
	.type	KOLIBA_IsIdentityFlut, %function

KOLIBA_IsIdentityFlut:

	@ It is not an I FLUT if it is not a FLUT at all.

	subs		r1, r0, 0
	mov			r0, 0
	movne		r2, 3
	bxeq		lr

	@ In the first half of I FLUT there are three ones.
	@ All the rest are zeros.

	vldr.f64	d1, .One

.BigLoop:

	mov			r3, 3

.SmallLoop:

	vldmia.f64	r1!, {d0}
	vcmp.f64	d0, #0
	vmrs		APSR_nzcv, FPSCR
	bxne		lr
	subs		r3, r3, 1
	bne			.SmallLoop

	vldmia.f64	r1!, {d0}
	vcmp.f64	d0, d1
	vmrs		APSR_nzcv, FPSCR
	bxne		lr
	subs		r2, r2, 1
	bne			.BigLoop

	mov			r2, 12

.FinalLoop:

	vldmia.f64	r1!, {d0}
	vcmp.f64	d0, #0
	vmrs		APSR_nzcv, FPSCR
	bxne		lr
	subs		r2, r2, 1
	bne			.FinalLoop

	mov			r1, 0
	bx			lr

	.align		

.One:

	.double		1.0


	.size	KOLIBA_IsIdentityFlut, .-KOLIBA_IsIdentityFlut
