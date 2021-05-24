/*

	matreset.s - ARM EABI 32-bit version

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
	@		R0 = address of KOLIBA_MATRIX

	.arch	armv6
	.text
	.align	3
	.syntax	unified
	.arm
	.fpu	vfp

.FindIdentityMatrix:

	ldr		r3, .Got
	ldr		r2, .IdMatrix

.HerrGot:

	add		r3, pc, r3
	ldr		r2, [r3, r2]
	bx	lr

	.align	3

.Got:

	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IdMatrix:

	.word	KOLIBA_IdentityChannelBlend(GOT)


	.align	3
	.global	KOLIBA_ResetMatrix
	.type	KOLIBA_ResetMatrix, %function
KOLIBA_ResetMatrix:

	cmp		r0, 0
	movne	r1, lr
	bxeq	lr

	bl		.FindIdentityMatrix
	mov		r3, r0

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}
	vldmia.f64	r2, {d0-d3}
	vstmia.f64	r3, {d0-d3}

	bx			r1

	.size	KOLIBA_ResetMatrix, .-KOLIBA_ResetMatrix

	.align	3
	.global	KOLIBA_ResetMatrixRed
	.type	KOLIBA_ResetMatrixRed, %function

KOLIBA_ResetMatrixRed:

	cmp		r0, 0
	movne	r1, lr
	bxeq	lr

	bl		.FindIdentityMatrix

	vldmia.f64	r2, {d0-d3}
	vstmia.f64	r0, {d0-d3}

	bx			r1

	.size	KOLIBA_ResetMatrixRed, .-KOLIBA_ResetMatrixRed

	.align	3
	.global	KOLIBA_ResetMatrixGreen
	.type	KOLIBA_ResetMatrixGreen, %function

KOLIBA_ResetMatrixGreen:

	cmp		r0, 0
	movne	r1, lr
	bxeq	lr

	bl		.FindIdentityMatrix

	add		r2, r2, 4*8			@ Skip to green
	add		r3, r0, 4*8

	vldmia.f64	r2, {d0-d3}
	vstmia.f64	r3, {d0-d3}

	bx			r1

	.size	KOLIBA_ResetMatrixGreen, .-KOLIBA_ResetMatrixGreen

	.align	3
	.global	KOLIBA_ResetMatrixBlue
	.type	KOLIBA_ResetMatrixBlue, %function
KOLIBA_ResetMatrixBlue:

	cmp		r0, 0
	movne	r1, lr
	bxeq	lr

	bl		.FindIdentityMatrix

	add		r2, r2, 2*4*8		@ Skip to blue
	add		r3, r0, 2*4*8

	vldmia.f64	r2, {d0-d3}
	vstmia.f64	r3, {d0-d3}

	bx			r1

	.size	KOLIBA_ResetMatrixBlue, .-KOLIBA_ResetMatrixBlue
