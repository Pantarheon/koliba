/*

	yccmat.s - ARM EABI 32-bit version

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
	@		  R0    = address of output MATRIX
	@		  R1    = address of (const) input MATRIX
	@		  R2    = address of (const) RGB model
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX

	.arch 	armv6
	.text
	.align	2
	.global	KOLIBA_YccMatrix
	.arch 	armv6
	.syntax unified
	.arm
	.equ	matsize,	(3*4*8)
	.equ	lvars,	(2*matsize+4)
	.fpu vfp
	.type	KOLIBA_YccMatrix, %function

KOLIBA_YccMatrix:

	@ Save some registers and create local vars.
	@ Why are we not checking for NULL pointers?
	@ Because the functions we call do, and they
	@ even check for other impossible conditions.
	@
	@ So, if anything goes wrong, we do return NULL.

	push	{r0, r1, r2, r4, lr}
	sub		sp, sp, lvars
	mov		r1, r2			@ R1 = Rec
	mov		r0, sp			@ R0 = Matrix A
	bl		KOLIBA_RgbToYcc

	add		r2, sp, lvars+4	@ R2 = input
	mov		r1, r0			@ R1 = R0 = A
	bl		KOLIBA_MultiplyMatrices

	@ Even though R0 will usually still equal SP,
	@ it may also be NULL if something was wrong.
	@ So we save R0 in R4 and pass that to the
	@ final multiplication instead of just SP.

	mov		r4, r0

	add		r1, sp, lvars+8	@ R1 = Rec
	add		r0, sp, matsize	@ R0 = Matrix B
	bl		KOLIBA_YccToRgb

	mov		r2, r0			@ R2 = YccToRgb matrix
	mov		r1, r4			@ R1 = A = input * RgbToYcc matrices
	ldr		r0, [sp, lvars]	@ R0 = output
	bl		KOLIBA_MultiplyMatrices

	@ Hasta la Pasta!

	add		sp, sp, lvars+3*4
	pop		{r4, pc}

	.size	KOLIBA_YccMatrix, .-KOLIBA_YccMatrix
