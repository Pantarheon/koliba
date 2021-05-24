/*

	mulmats.s - ARM EABI 32-bit version

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
	@		R0 = address of output ("matrix C")
	@		R1 = address of (const) multiplicand ("matrix B")
	@		R2 = address of (const) multiplier ("matrix A")
	@
	@	A, B, C are 3x4 matrices (3 rows, 4 columns each).
	@
	@	We calculate the matrix product C = AB, and return C.
	@
	@	Sound impossible? It might be, except in our case it is not.
	@	Why? Becaue our 3x4 matrices are actually "compressed"
	@	4x4 square matrices, where the 4th row is implied to be [0 0 0 1].
	@	And when we multiply such matrices, the 4th row of the result
	@	also is [0 0 0 1].
	@
	@	As always in this library, we compute the result on the stack
	@	first, then copy it to matrix C. That allows us to reuse either
	@	of the input matrices as the output matrix, if that is what we
	@	want or need.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_MultiplyMatrices
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_MultiplyMatrices, %function

	@ Define the size of a matrix, so we do not mess up
	@ by typing a wrong number
	.equ	MatSize,	(3*4*8)

KOLIBA_MultiplyMatrices:

	@ If any of the arguments is NULL, return NULL.

	cmp			r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Else, do the work.

	vpush.64	{d8-d15}
	sub			sp, sp, MatSize
	mov			r12, sp				@ Create a temporary matrix

	@ Load the entire matrix A to the registers.
	vldmia.64	r2, {d4-d15}

	@ We no longer need the address of matrix A (since it is in registers),
	@ so we can reuse R2 as a counter. We will compute the entire column
	@ at a time, and since we have four columns...

	mov			r2, 4

	@ Engage!

.LLoop:

	@ for (j = 4; j > 0; j--)
	subs		r2, r2, 1		@ Are we at the last column yet?

	@ The last column is special, so treat it differently.

	vsubne.f64	d0, d0, d0		@ If not last column, D0 = 0
	vmoveq.f64	d0, d7			@ Else, D0 = A_{1,4}
	vmovne.f64	d1, d0			@ If not last column, D1 = 0
	vmoveq.f64	d1, d11			@ Else, D1 = A_{2,4}
	vmovne.f64	d2, d0			@ If not last column, D2 = 0
	vmoveq.f64	d2, d15			@ Else, D2 = A_{3,4}

	@ Accumulate [C_{1,j}, C_{2,j}, C_{3,j}] in D0-D2.

	vldr.64		d3, [r1]		@ D3  = B_{1,j}
	vmla.f64	d0, d3, d4		@ D0 += A_{1,1} * B{1,j}
	vmla.f64	d1, d3, d8		@ D0 += A_{2,1} * B{1,j}
	vmla.f64	d2, d3, d12		@ D0 += A_{3,1} * B{1,j}

	vldr.64		d3, [r1,32]		@ D3  = B_{2,j}
	vmla.f64	d0, d3, d5		@ D0 += A_{1,2} * B{2,j}
	vmla.f64	d1, d3, d9		@ D0 += A_{2,2} * B{2,j}
	vmla.f64	d2, d3, d13		@ D0 += A_{3,2} * B{2,j}

	vldr.64		d3, [r1,64]		@ D3  = B_{3,j}
	vmla.f64	d0, d3, d6		@ D0 += A_{1,3} * B{3,j}
	vmla.f64	d1, d3, d10		@ D0 += A_{2,3} * B{3,j}
	vmla.f64	d2, d3, d14		@ D0 += A_{3,3} * B{3,j}

	@ Store the computed column in the stack.

	vstr.64		d0, [r12]
	vstr.64		d1, [r12,32]
	vstr.64		d2, [r12,64]

	@ Loop to next column. The flags still reflect the subs op code way above.

	addne		r1, 8
	addne		r12, 8
	bne			.LLoop

	@ Copy the result from the stack to the output (matrix C).

	vldmia.f64	sp, {d4-d15}
	vstmia.f64	r0, {d4-d15}

	@ All done. Clean up and return.

	add			sp, sp, MatSize
	vpop.f64	{d8-d15}
	bx			lr

	.size	KOLIBA_MultiplyMatrices, .-KOLIBA_MultiplyMatrices
