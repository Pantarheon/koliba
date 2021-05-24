/*

	sltreset.s - ARM EABI 32-bit version

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
	@		R0 = address of output sLut
	@
	@	Various routines to set a sLut, or a portion
	@	thereof, to the identity state.

	.arch armv6
	.text
	.align		2
	.global		KOLIBA_ResetSlut
	.arch		 armv6
	.syntax		unified
	.arm
	.fpu 		vfp
	.type		KOLIBA_ResetSlut, %function

KOLIBA_ResetSlut:

	mov			r12, lr
	mov			r3, r0
	bl			.GetIdentitySlut

	@ If we ever return here, the sLut is in R0 and R3,
	@ the identity sLut table is in R1, the return
	@ address still in R12. We copy the whole table.
	@ The easiest way to do it is by using D0-D7.

	vldmia.f64	r1!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}
	vldmia.f64	r1!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}
	vldmia.f64	r1,  {d0-d7}
	vstmia.f64	r3,  {d0-d7}

	@ Hasta la Pasta!

	bx			r12

	.size	KOLIBA_ResetSlut, .-KOLIBA_ResetSlut

	.align	2
	.global	KOLIBA_ResetSlutBlack
	.type	KOLIBA_ResetSlutBlack, %function

KOLIBA_ResetSlutBlack:

	mov			r12, lr
	bl			.GetIdentitySlut

	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r0, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutBlack, .-KOLIBA_ResetSlutBlack

	.align	2
	.global	KOLIBA_ResetSlutBlue
	.type	KOLIBA_ResetSlutBlue, %function

KOLIBA_ResetSlutBlue:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24
	add			r2, r0, 24
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutBlue, .-KOLIBA_ResetSlutBlue

	.align	2
	.global	KOLIBA_ResetSlutGreen
	.type	KOLIBA_ResetSlutGreen, %function

KOLIBA_ResetSlutGreen:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 48
	add			r2, r0, 48
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutGreen, .-KOLIBA_ResetSlutGreen

	.align		2
	.type		.GetIdentitySlut, %function

.GetIdentitySlut:


	cmp			r0, 0
	ldr			r2, .Got
	bxeq		r12
	ldr			r1, .IDSlut

.HerrGot:

	add			r2, pc, r2
	ldr			r1, [r2, r1]
	bx			lr

	.align		2

.Got:

	.word		_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IDSlut:

	.word		KOLIBA_IdentitySlut(GOT)

	.size		.GetIdentitySlut, .-.GetIdentitySlut

	.align	2
	.type	KOLIBA_ResetSlutCyan, %function

KOLIBA_ResetSlutCyan:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24*3
	add			r2, r0, 24*3
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutCyan, .-KOLIBA_ResetSlutCyan

	.align	2
	.global	KOLIBA_ResetSlutRed
	.type	KOLIBA_ResetSlutRed, %function

KOLIBA_ResetSlutRed:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24*4
	add			r2, r0, 24*4
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutRed, .-KOLIBA_ResetSlutRed

	.align	2
	.global	KOLIBA_ResetSlutMagenta
	.type	KOLIBA_ResetSlutMagenta, %function

KOLIBA_ResetSlutMagenta:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24*5
	add			r2, r0, 24*5
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutMagenta, .-KOLIBA_ResetSlutMagenta

	.align	2
	.global	KOLIBA_ResetSlutYellow
	.type	KOLIBA_ResetSlutYellow, %function

KOLIBA_ResetSlutYellow:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24*6
	add			r2, r0, 24*6
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutYellow, .-KOLIBA_ResetSlutYellow

	.align	2
	.global	KOLIBA_ResetSlutWhite
	.type	KOLIBA_ResetSlutWhite, %function

KOLIBA_ResetSlutWhite:

	mov			r12, lr
	bl			.GetIdentitySlut

	add			r1, r1, 24*7
	add			r2, r0, 24*7
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	bx			r12

	.size	KOLIBA_ResetSlutWhite, .-KOLIBA_ResetSlutWhite
