/*

	gsmat.s - ARM EABI 32-bit version

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
	@		  R1    = address of (const) RGB Rec
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX or NULL

	.arch	armv6
	.text
	.align	2
	.global	KOLIBA_GrayscaleMatrix
	.syntax	unified
	.arm
	.fpu	vfp
	.type	KOLIBA_GrayscaleMatrix, %function

KOLIBA_GrayscaleMatrix:

	@ If either pointer is NULL, return NULL.

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Load the Rec and add it up. If it comes to 0, return NULL.

	vldmia.f64	r1, {d0-d2}

	vmov.f64	d3, d0
	vadd.f64	d3, d1, d3
	vadd.f64	d3, d2, d3
	vcmp.f64	d3, #0
	vmrs		APSR_nzcv, FPSCR
	moveq		r0, 0
	bxeq		lr

	@ Divide D0 and D1 by the sum,
	@ then store 1 - D0 - D1 in D2,
	@ and 0 in D3.

	vdiv.f64	d0, d0, d3
	vldr.f64	d2, .One
	vdiv.f64	d1, d1, d3
	vsub.f64	d2, d2, d1
	vsub.f64	d3, d3, d3
	mov			r1, r0
	vsub.f64	d2, d2, d0

	@ Store {D0-D3} in all three rows of the matrix.

	vstmia.f64	r1!, {d0-d3}
	vstmia.f64	r1!, {d0-d3}
	vstmia.f64	r1, {d0-d3}

	@ Hasta la Pasta!

	bx			lr


	.align	3

.One:

	.double	1.0

	.size	KOLIBA_GrayscaleMatrix, .-KOLIBA_GrayscaleMatrix
