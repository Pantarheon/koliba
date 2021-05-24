/*

	checksum.s - ARM EABI 32-bit version

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
	@		D0 = checksum
	@
	@	Verify the checksum in D0 is correct for the data.
	@	Return true if it is, false if it is not or if
	@	no data has been passed.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_CheckSum
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_CheckSum, %function

KOLIBA_CheckSum:

	mov				r2, r0
	mov				r0, 0
	cmp				r1, 0
	cmpne			r2, 0
	bxeq			lr

	@ Use D1 as accumulator, D2 as an index, D3 for data, and D4 as 1.0
	vmov.u32		s2, r0
	mov				r3, 1
	vcvt.f64.u32	d1, s2				@ D1  = 0.0;
	vmov.u32		s8, r3
	vmov.f64		d2, d1
	vcvt.f64.u32	d4, s8

.LLoop:

	vadd.f64		d2, d2, d4			@ D2  = (double)(i+1);
	vldmia.f64		r2!, {d3}			@ D3  = (double)array[i];
	subs			r1, r1, 1
	vmla.f64		d1, d2, d3			@ D1 += (i+1) * array[i];
	bne				.LLoop				@ (i < n) ? loop : break;

	vcmp.f64		d0, d1
	vmrs			APSR_nzcv, fpscr
	moveq			r0, 1				@ return (acc == csum) ? true : false;
	bx				lr

	.size	KOLIBA_CheckSum, .-KOLIBA_CheckSum
