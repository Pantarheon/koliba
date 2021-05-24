/*

	calcsum.s - ARM EABI 32-bit version

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
	@		R0 = address of (const) double array
	@		R1 = number of doubles in the above array

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_CalcSum
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_CalcSum, %function

KOLIBA_CalcSum:

	@ Return 0.0 if there is nothing to do.

	vsub.f64		d0, d0, d0			@ D0  = 0.0;
	cmp				r1, 0
	cmpne			r0, 0
	bxeq			lr

	@ Use D0 as accumulator, D1 as an index, D2 for data, and D3 as 1.0

	mov				r3, 1
	vmov.f64		d1, d0				@ D1  = 0.0;
	vmov.u32		s8, r3
	vcvt.f64.u32	d3, s8

.LLoop:

	vadd.f64		d1, d1, d3			@ D1  = (double)(i+1);
	vldmia.f64		r0!, {d2}			@ D2  = (double)array[i];
	subs			r1, r1, 1
	vmla.f64		d0, d1, d2			@ D0 += (i+1) * array[i];
	bne				.LLoop				@ (i < n) ? loop : break;

	bx				lr

	.size	KOLIBA_CalcSum, .-KOLIBA_CalcSum
