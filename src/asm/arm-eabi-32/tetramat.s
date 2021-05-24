/*

	tetramat.s - ARM EABI 32-bit version

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
	@		  R1    = address of (const) Y MATRIX
	@		  R2    = address of (const) R matrix
	@		  R3    = address of (const) G matrix
	@		 [SP]   = address of (const) B matrix
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_TetraMatrix
	.arch	armv6
	.syntax unified
	.arm
	.equ	matsize,	(3*4*8)
	.equ	lvars,	matsize
	.fpu 	vfp
	.type	KOLIBA_TetraMatrix, %function

KOLIBA_TetraMatrix:

	@ Make sure R, G, B are not NULL.

	cmp			r3, 0
	ldrne		r12, [sp]
	addne		r3, r3, 4*8
	cmpne		r2, 0
	cmpne		r12, 0
	moveq		r0, 0
	addne		r12, r12, 2*4*8
	bxeq		lr

	@ Save LR and create a local matrix.
	@ But keep the stack 8-byte aligned.

	str			lr, [sp, -8]!		@ 8-byte "push" of 4-byte register

	@ Copy the relevant rows from R, G, B to temp T.

	vldmia.f64	r12, {d4-d7}		@ B.b
	vldmia.f64	r3, {d0-d3}			@ G.g
	vpush.f64	{d0-d7}
	vldmia.f64	r2, {d0-d3}			@ R.r
	vpush.f64	{d0-d3}

	@ Multiply Y by T.

	mov			r2, sp
	bl			KOLIBA_MultiplyMatrices

	@ Hasta la Pasta!

	add			sp, sp, lvars
	ldr			pc, [sp], 8			@ 8-byte "pop" of 4-byte register

	.size	KOLIBA_TetraMatrix, .-KOLIBA_TetraMatrix
