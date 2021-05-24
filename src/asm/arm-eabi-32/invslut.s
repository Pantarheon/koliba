/*

	invslut.s - ARM EABI 32-bit version

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
	@		R0 = address of sLut to potentially modify
	@		R1 = byte of flags
	@
	@ On Output:
	@
	@		R0 = address same sLut
	@
	@	We subtract the vertices the flags identify
	@	from 1.0, thus inverting them (making their
	@	negative.)

	.arch 	armv6
	.text
	.align	2
	.global	KOLIBA_InvertSlutVertices
	.syntax unified
	.arm
	.fpu 	vfp
	.type	KOLIBA_InvertSlutVertices, %function

KOLIBA_InvertSlutVertices:

	mov				r3, 0x3f800000
	cmp				r0, 0
	mov				r2, 8
	vmovne			s0, r3
	bxeq			lr
	vcvt.f64.f32	d3, s0		@ D3 = 1.0

.Loop:

	lsrs			r1, r1, 1
	vldmiacs.f64	r0, {d0-d2}
	vsubcs.f64		d0, d3, d0
	vsubcs.f64		d1, d3, d1
	vsubcs.f64		d2, d3, d2
	vstmiacs.f64	r0, {d0-d2}
	subs			r2, r2, 1
	add				r0, r0, 3*8
	bxeq			lr
	b				.Loop

	.size	KOLIBA_InvertSlutVertices, .-KOLIBA_InvertSlutVertices
