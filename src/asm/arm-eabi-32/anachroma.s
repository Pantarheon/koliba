/*

	anachroma.s - ARM EABI 32-bit version

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
	@		  R1    = address of (const) ANACHROMA
	@		  R2    = unsigned int normalize (boolean)
	@		  R3    = unsigned int channel {0-2}
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX or NULL

	.text
	.align	2
	.global	KOLIBA_AnachromaticMatrix
	.arch	armv6
	.syntax unified
	.arm
	.equ	angle,	0
	.equ	magnitude, (angle+8)
	.equ	saturation,	(magnitude+8)
	.equ	black,	(saturation+8)
	.equ	white,	(black+8)
	.equ	chroma,	(white+8)
	.equ	rgb,	(3*8)
	.equ	matrow,	(4*8)
	.equ	matsize,	(3*matrow)
	.equ	red,	0
	.equ	green,	(red+matrow)
	.equ	blue,	(green+matrow)
	.equ	rgbmodel,	0
	.equ	chromatchroma,	(rgbmodel+rgb)
	.equ	chromat, (chromatchroma+chroma)
	.equ	anachr0,	(rgbmodel+rgb)
	.equ	anachr1,	(anachr0+chroma)
	.equ	rotation,	(anachr1+chroma)
	.equ	efficacy,	(rotation+8)
	.equ	anachroma,	(efficacy+8)
	.equ	m,	0
	.equ	m1, (m+matsize)
	.equ	chrm,	(m1+matsize)
	.equ	lvars, (chrm+chromat)
	.fpu	vfp
	.type	KOLIBA_AnachromaticMatrix, %function

KOLIBA_AnachromaticMatrix:

	@ If the output matrix or the anachroma
	@ are NULL, or channel > 2, return NULL

	cmp			r3, 2
	movhi		r0, 0
	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Save registers and create local vars

	push		{r0, r4-r9, lr}
	sub			sp, sp, lvars

	add			r9, sp, chrm	@ R9 = chromat
	add			r8, sp, m1		@ R8 = M1
	mov			r7, sp			@ R7 = M
	push		{r1-r3}
	pop			{r4-r6}			@ R4 = anachroma, R5 = normalize, R6 = channel

	mov			r0, r9
	bl			KOLIBA_ResetChromaticMatrix

	mov			r2, r4			@ R2 = anachroma.model
	add			r1, r4, anachr0	@ R1 = anachroma.chr[0]
	mov			r0, r8			@ R0 = M1
	bl			KOLIBA_ChromaMatrix

	@ Set the saturation of chroma to 0, and convert chrm to matrix M.

	add			r12, r9, chromatchroma+saturation
	mov			r3, 0
	mov			r2, 0
	mov			r1, r9
	mov			r0, r7
	stmia		r12, {r2-r3}
	bl			KOLIBA_ChromaticMatrix

	@ If channel <> 0, copy M1.red, else M1.green, to M
	@ and normalize the row.

	cmp			r6, 0
	movne		r12, r8
	addeq		r12, r8, green
	movne		r0, r7
	addeq		r0, r7, green

	vldmia.f64	r12, {d0-d3}
	mov			r1, r5			@ R1 = normalize
	vstmia.f64	r0, {d0-d3}		@ R0 = M.red or M.green
	bl			KOLIBA_NormalizeMatrixRow

	mov			r2, r4			@ R2 = anachroma.model
	add			r1, r4, anachr1	@ R1 = anachroma.chr[1]
	mov			r0, r8			@ R0 = M1
	bl			KOLIBA_ChromaMatrix

	@ If channel > 1, copy M1.green, else M1.blue,
	@ to M and normalize the row.

	cmp			r6, 1
	addls		r12, r8, blue
	addhi		r12, r8, green
	addls		r0, r7, blue
	addhi		r0, r7, green

	vldmia.f64	r12, {d0-d3}
	mov			r1, r5			@ R1 = normalize
	vstmia.f64	r0, {d0-d3}		@ R0 = M.blue or M.green
	bl			KOLIBA_NormalizeMatrixRow

	@ If anachroma rotation is not 0, apply it.

	vldr.f64	d0, [r4, rotation]
	vcmp.f64	d0, #0
	vmrs		APSR_nzcv, FPSCR
	vldrne.f64	d1, .One
	beq			.Efficacy

	vstr.f64	d0, [r9, chromatchroma+angle]
	mov			r1, r9
	vstr.f64	d1, [r9, chromatchroma+saturation]
	mov			r0, r8
	bl			KOLIBA_ChromaticMatrix

	mov			r2, r0
	mov			r1, r7
	mov			r0, r7
	bl			KOLIBA_MultiplyMatrices

.Efficacy:

	@ Apply efficacy.

	mov			r1, r7
	ldr			r0, [sp, lvars]
	vldr.f64	d0, [r4, efficacy]
	bl			KOLIBA_MatrixEfficacy

	@ Hasta la Pasta!

	add			sp, sp, lvars+4
	pop			{r4-r9, pc}

	.align	3

.One:

	.double	1.0


	.size	KOLIBA_AnachromaticMatrix, .-KOLIBA_AnachromaticMatrix
