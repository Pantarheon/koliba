/*

	matrixgain.s - ARM EABI 32-bit version

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
	@		R0 = address of output matrix
	@		R1 = address of (const) input modificand matrix, or NULL
	@		R2 = address of (const) input modifier gains vertex, or NULL

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.equ	r,	0
	.equ	g,	(r+8)
	.equ	b,	(g+8)
	.equ	o,	(b+8)
	.equ	row,	(o+8)
	.equ	red,	0
	.equ	green,	(red+row)
	.equ	blue,	(green+row)
	.equ	matrix,	(blue+row)
	.equ	lvars,	(matrix+4)

	.align	2
	.global	KOLIBA_MatrixGain
	.type	KOLIBA_MatrixGain, %function

KOLIBA_MatrixGain:

	@ If the output is NULL, all we can do is throw up hands

	cmp		r0, 0
	bxeq	lr

	@ Save some registers and set up a temporary matrix.

	push		{r0, r4, r5, r6, r7, r8, lr}
	sub			sp, sp, lvars

	@ If the input is NULL, use I.

	cmp			r1, 0
	ldr			r5, .Zeroes
	ldr			r12, .Got
	ldreq		r4, .IdMatrix
	movne		r4, r1

.HerrGot:

	add			r12, pc, r12
	ldreq		r4, [r12, r4]	@ R4 = input or I
	cmp			r2, 0
	ldr			r5, [r12, r5]	@ R5 = Zeroes
	ldreq		r6, .Ones
	movne		r6, r2
	ldreq		r6, [r12, r6]	@ R6 = gains, or ones
	mov			r7, sp			@ R7 = temporary matrix
	mov			r8, 3			@ R8 = "i" (counter)

.TheLoop:

	mov			r3, 3			@ Number of doubles to interpolate
	mov			r2, r5			@ Zeroes
	vldmia.f64	r6!, {d0}		@ gains->current
	mov			r1, r4			@ input->current
	mov			r0, r7			@ temp->current
	bl			KOLIBA_Interpolate

	vldr.f64	d0, [r4, o]		@ input->current.o
	subs		r8, 1			@ i--
	vstr.f64	d0, [r7, o]		@ temp->current.o
	addne		r4, r4, row		@ input->next
	addne		r7, r7, row		@ temp->next
	bne			.TheLoop		@ i > 0

	@ Copy the temporary matrix to the output matrix.

	ldr			r2, [sp, lvars]	@ output

	vldmia.f64	sp!, {d0-d8}
	vstmia.f64	r2!, {d0-d8}
	vldmia.f64	sp!, {d0-d3}
	vstmia.f64	r2,  {d0-d3}

	@ Hasta la Pasta!

	add			sp, sp, 4		@ 4 was alignment, we popped the matrix already
	pop			{r0, r4-r8, pc}


	.align 	2

.Ones:

	.word	KOLIBA_Ones(GOT)

.Zeroes:

	.word	KOLIBA_Zeroes(GOT)

.Got:

	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IdMatrix:

	.word	KOLIBA_IdentityChannelBlend(GOT)

	.size	KOLIBA_MatrixGain, .-KOLIBA_MatrixGain
