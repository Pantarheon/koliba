/*

	fltismat.s - ARM EABI 32-bit version

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
	@		R0 = address of (const) input fLut

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_FlutIsMatrix
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_FlutIsMatrix, %function

KOLIBA_FlutIsMatrix:

	@ A FLUT is a matrix if its upper half is all zeroes.
	@ Otherwise, it is not. Also, if there is no FLUT,
	@ there is no matrix, so check for that first.

	cmp				r0, 0
	bxeq			lr

	add				r1, r0, 12*8		@ Start of second half of the fLut.
	mov				r2, 12				@ Number of doubles in second half.

.Loop:

	vldmia.f64		r1!, {d0}
	vcmp.f64		d0, #0.0
	vmrs			APSR_nzcv, fpscr
	movne			r0, 0
	bxne			lr

	subs			r2, r2, 1
	bne				.Loop

	mov				r0, 1
	bx				lr

	.size	KOLIBA_FlutIsMatrix, .-KOLIBA_FlutIsMatrix
