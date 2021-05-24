/*

	flutter.s - ARM EABI 32-bit version

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
	@		R0 = address of output fLut
	@		R1 = address of (const) input modificand fLut
	@		R2 = address of (const) input modifier fLut

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Flutter
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Flutter, %function

KOLIBA_Flutter:

	@ Reject any NULL pointer.

	cmp				r2, 0
	cmpne			r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Set up local fLut.

	push			{r4, lr}
	vpush			{d8-d14}
	add				r3, r2, 3*8
	ldr				r4, .LZeroes
	sub				sp, sp, 3*8*8

	@ Load invariant parameters.

	vldmia.f64		r3, {d6-d14}

	@ Internal variables.

	add				r3, r2, 12*8
	mov				r12, sp

	@ Now do the math.

	bl				.LMath					@ Black

.LZeros:

	add				r2, pc, r4
	mov				r4, 3

.LRgbLoop:

	bl				.LMath					@ Red, green, blue
	subs			r4, r4, 1
	bne				.LRgbLoop

	mov				r2, r3
	mov				r3, 4

.Loop:

	bl				.LMath					@ Yellow, magenta, cyan, white
	subs			r3, r3, 1
	bne				.Loop

	@ Copy the results from the stack to the output.

	mov				r1, r0
	vldmia.f64		sp!, {d0-d11}
	vstmia.f64		r1!, {d0-d11}
	vldmia.f64		sp!, {d0-d11}
	vstmia.f64		r1, {d0-d11}

	@ Clean up and return.

	vpop			{d8-d14}
	pop				{r4, pc}

.LMath:

	vldmia.f64		r2!, {d0-d2}
	vldmia.f64		r1!, {d3-d5}

	vmla.f64		d0, d3, d6
	vmla.f64		d1, d3, d7
	vmla.f64		d2, d3, d8

	vmla.f64		d0, d4, d9
	vmla.f64		d1, d4, d10
	vmla.f64		d2, d4, d11

	vmla.f64		d0, d5, d12
	vmla.f64		d1, d5, d13
	vmla.f64		d2, d5, d14

	vstmia.f64		r12!, {d0-d2}

	bx				lr


	.align	2

.LZeroes:

	.word	KOLIBA_Zeroes-(.LZeros+8)

	.size	KOLIBA_Flutter, .-KOLIBA_Flutter

	.hidden	KOLIBA_Zeroes
