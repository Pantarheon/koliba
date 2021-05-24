/*

	chroma.s - ARM EABI 32-bit version

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
	@		  R1    = address of (const) input CHROMAT
	@
	@ On Exit:
	@
	@		 R1    = address of CHROMAT.chroma
	@		 R2    = address of CHROMAT.model
	@
	@	The fall through...

	@ On Entry:
	@
	@		  R0    = address of output MATRIX
	@		  R1    = address of (const) input CHROMA
	@		  R2    = address of (const) RGB model
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX

	.arch	 armv6
	.text
	.align	3
	.global	KOLIBA_ChromaticMatrix
	.global	KOLIBA_ChromaMatrix
	.arch 	armv6
	.syntax unified
	.arm
	.fpu 	vfp
	@ Four doubles {r,g,b,o} in a matrix row.
	.equ	matrow,	(4*8)
	@ Three rows {red,green,blue} in a matrix (the fourth row is implied but not stored).
	.equ	matsize,	(3*matrow)
	.equ	red,	0
	.equ	green,	(red+matrow)
	.equ	blue,	(green+matrow)
	.equ	r,	0
	.equ	g,	8
	.equ	b,	16
	.equ	o,	24
	@ KOLIBA_CHROMA = {double angle, magnitude, saturation, black, white};
	.equ	angle,	0
	.equ	magnitude, (angle+8)
	.equ	saturation,	(magnitude+8)
	.equ	black,	(saturation+8)
	.equ	white,	(black+8)
	.equ	chrsize,	(white+8)
	.equ	matrixC,	0
	.equ	matrixB,	(matrixC+matsize)
	.equ	matrixA,	(matrixB+matsize)
	.equ	mats,	(matrixA+matsize)
	.type	KOLIBA_ChromaticMatrix, %function
	.type	KOLIBA_ChromaMatrix, %function

KOLIBA_ChromaticMatrix:

	@ Reject NULL pointer to CHROMAT but leave it up
	@ to the next function to reject NULL output, if any.
	@
	@ The conditional nop is there, so KOLIBA_ChromaMatrix
	@ starts at an 8-byte boundary.

	cmp			r1, 0
	nopeq
	mov			r2, r1			@ R2 = address of CHROMAT.model, or NULL
	addne		r1, r1, 3*8		@ R1 = address of CHROMAT.chroma

	@ Continue in KOLIBA_ChromaMatrix

KOLIBA_ChromaMatrix:

	@ Reject NULL pointers

	cmp			r2, 0
	cmpne		r1, 0
	vldrne.f64	d0, [r1]			@ D0 = angle in degrees
	moveq		r0, 0
	vldrne.f64	d1, .PiDiv180		@ D1 = pi / 180
	cmpne		r0, 0
	vldrne.f64	d2, [r1,black]		@ D2 = black
	bxeq		lr

	vldr.f64	d3, [r1,white]
	push		{r0, r1, r4, lr}
	vsub.f64	d3, d3, d2			@ D3 = white - black
	sub			sp, sp, mats

	@ Convert angle from degrees to radians,
	@ and calculate sine and cosine.

	vmul.f64	d0, d1, d0
	vstr.f64	d2, [sp,red+o]
	add			r1, sp, green+g		@ cos
	vstr.f64	d3, [sp,red+r]
	add			r0, sp, green+b		@ sin
	mov			r4, r2				@ R4 = model
	bl			sincos				@ from GNU C

	add			r12, sp, green+g
	vldmia.f64	r12, {d1-d2}		@ D1 = cos, D2 = sin
	add			r12, r12, blue-green
	vneg.f64	d0, d2
	vstmia.f64	r12, {d0-d1}

	vsub.f64	d0, d0
	add			r12, sp, red+g
	vmov.f64	d1, d0
	vstr.f64	d0, [sp,blue+o]
	vstmia.f64	r12, {d0-d1}
	add			r12, r12, green+o - (red+g)
	vstr.f64	d0, [sp,green+r]
	vstmia.f64	r12, {d0-d1}

	@ In-place interpolate the green and blue data of Matrix C
	@ with the corresponding identity values using magnitude.

	ldr			r12, [sp, mats+4]	@ R12 = chroma
	mov			r3, 8
	adr			r2, .Identity
	add			r1, sp, green
	vldr.f64	d0, [r12,magnitude]
	mov			r0, r1
	bl			KOLIBA_Interpolate

	@ Multiply green[g,b] and blue[g,b] by saturation.
	@ We can *probably* speed it up if we also load
	@ the green.o and blue.r values even though we
	@ do not modify them.
	@
	@ The call to KolibaInterpolate has returned the address
	@ of green, so we can just add 8 to r0 to get address of
	@ green.g.
	@
	@ After that convert the model to an YCC-to-RGB matrix
	@ in matrix A.

	ldr			r12, [sp, mats+4]	@ R12 = chroma
	add			r0, r0, 8
	vldr.f64	d0, [r12,saturation]
	vldmia.f64	r0, {d1-d6}
	vmul.f64	d1, d0, d1			@ D1 = green.g
	vmul.f64	d2, d0, d2			@ D2 = green.b
	vmul.f64	d5, d0, d5			@ D5 = blue.g
	vmul.f64	d6, d0, d6			@ D6 = blue.b
	mov			r1, r4				@ R1 = model
	vstmia.f64	r0, {d1-d6}
	add			r0, sp, matrixA
	bl			KOLIBA_YccToRgb

	@ And create the RGB-to-YCC matrix in matrix B.

	mov			r1, r4				@ R1 = model
	add			r0, sp, matrixB
	bl			KOLIBA_RgbToYcc

	@ Multiply the result (multiplicand), which is in R0 right now
	@ by matrix C (multiplier) and store the result in C.

	mov			r2, sp				@ R2 = matrix C (multiplier)
	mov			r1, r0				@ R1 = matrix B (multiplicand)
	mov			r0, r2				@ R0 = matrix C = C*B
	bl			KOLIBA_MultiplyMatrices

	@ Multiply the result (multiplicand) by YCC-to-RGB (matrix A)
	@ and store the result in the output.

	add			r2, sp, matrixA		@ R2 = matrix A (multiplier)
	mov			r1, r0				@ R1 = C*B (multiplicand)
	ldr			r0, [sp, mats]		@ R0 = output = A*C*B
	bl			KOLIBA_MultiplyMatrices

	@ Hasta la Pasta!

	add			sp, sp, mats+8		@ No need to pop R0 or R1
	pop			{r4, pc}


	.align	3

.Identity:

	.double	0.0, 1.0, 0.0, 0.0
	.double	0.0, 0.0, 1.0, 0.0

.PiDiv180:

	.word	0xA2529D39, 0x3F91DF46

	.size	KOLIBA_ChromaMatrix, .-KOLIBA_ChromaMatrix
	.size	KOLIBA_ChromaticMatrix, .-KOLIBA_ChromaticMatrix
