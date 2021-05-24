/*

	cb2mat.s - ARM EABI 32-bit version

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
	@		R1 = address of (const) input channel blend

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.equ	row,	(4*8)
	.equ	red,	0
	.equ	green,	(red+row)
	.equ	blue,	(green+row)
	.equ	matrix,	(blue+row)
	.equ	efficacy,	(matrix)
	.equ	flags,	(efficacy+8)
	.equ	lvars,	(matrix)

	.align	3
	.global	KOLIBA_ConvertChannelBlendToMatrix
	.type	KOLIBA_ConvertChannelBlendToMatrix, %function

KOLIBA_ConvertChannelBlendToMatrix:

	@ Reject NULL pointers

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Save registers, set up local variables.

	push		{r0, r1, r4, lr}
	ldr			r4, [r1, flags]			@ R4 = flags
	sub			sp, sp, lvars

	@ Copy the matrix from the blend to temporary matrix

	mov			r12, sp
	vldmia.f64	r1!, {d0-d7}
	vstmia.f64	r12!, {d0-d7}
	vldmia.f64	r1, {d0-d3}
	vstmia.f64	r12, {d0-d3}

	@ If the flags warrant, normalize temp.red.

	uxth		r1, r4, ror 24
	cmp			r1, 0
	movne		r0, sp
	blne		KOLIBA_NormalizeMatrixRow

	@ If the flags warrant, normalize temp.green.
	uxtb16		r1, r4, ror 8
	cmp			r1, 0
	addne		r0, sp, green
	blne		KOLIBA_NormalizeMatrixRow

	@ If the flags warrant, normalize temp.blue

	uxth		r1, r4, ror 16
	cmp			r1, 0
	addne		r0, sp, blue
	blne		KOLIBA_NormalizeMatrixRow

	@ Interpolate temporary matrix with I and send the result to the output.

	adr			r12, .IdMatrix
	ldr			r1, [sp, lvars+4]	@ R1 = address of channel blend
	ldmia		r12, {r2, r3}		@ R2 = .IdMatrix, R3 = .Got
	vldr.f64	d0, [r1, efficacy]	@ D0 = blend efficacy

.HerrGot:

	add			r3, pc, r3			@ R3 = GOT
	ldr			r0, [sp, lvars]		@ R0 = output matrix
	mov			r1, sp				@ R1 = temporary matrix
	ldr			r2, [r3, r2]		@ R2 = I
	bl			KOLIBA_InterpolateMatrices

	@ Hasta la Pasta!

	add			sp, sp, lvars+8		@ No need to pop R0-R1 (R0 = return of interpolation).
	pop			{r4, pc}


	.align	3
	
.IdMatrix:

	.word	KOLIBA_IdentityChannelBlend(GOT)
	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

	.size	KOLIBA_ConvertChannelBlendToMatrix, .-KOLIBA_ConvertChannelBlendToMatrix
	