/*

	nmatrow.s - ARM EABI 32-bit version

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
	@		R0 = address of matrix row
	@		R1 = wade

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_NormalizeMatrixRow
	.type	KOLIBA_NormalizeMatrixRow, %function

KOLIBA_NormalizeMatrixRow:

	@ If wade is not zero and the row is not NULL,
	@ see if you can normalize the row.

	cmp			r1, 0
	cmpne		r0, 0
	bxeq		lr

	@ We can normalize if the sum of all four members
	@ of the row is not zero...

	vldmia.f64	r0, {d0-d3}
	vadd.f64	d4, d0, d1
	vadd.f64	d4, d2, d4
	vadd.f64	d4, d3, d4
	vcmp.f64	d4, #0
	vmrs		APSR_nzcv, FPSCR

	@ ...else we might normalize if sum - .o is not zero,
	@ but if so, we will also have to make .o = 0.

	vsubeq.f64	d4, d4, d3
	vcmpeq.f64	d4, #0
	vsubeq.f64	d3, d3, d3
	vmrseq		APSR_nzcv, FPSCR

	@ Else, we cannot normalize, so we just return.

	bxeq		lr

	@ Normalize by dividing the four members by the sum.
	@ It does not matter which of the two variations we
	@ are doing because in the first case we just divide
	@ by the sum, and in the second case we have adjusted
	@ both, the sum to sum - .o and .o to zero.

	vdiv.f64	d0, d0, d4
	vdiv.f64	d1, d1, d4
	vdiv.f64	d2, d2, d4
	vdiv.f64	d3, d3, d4
	vstmia.f64	r0, {d0-d3}

	@ Hasta la Pasta!

	bx			lr

	.size	KOLIBA_NormalizeMatrixRow, .-KOLIBA_NormalizeMatrixRow
