/*

	gray2flut.s - ARM EABI 32-bit version

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
	@		R0 = address of fLut
	@		R1 = address of (const) gray (KOLIBA_RGB)
	@
	@ On Exit:
	@
	@		R0 = address of fLut, or NULL
	@
	@	We only return NULL if fLut is NULL. If gray
	@	is NULL, we just use Rec. 2020.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertGrayToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertGrayToFlut, %function

KOLIBA_ConvertGrayToFlut:
	cmp				r0, 0

	ldr				r3, .LGot
	bxeq			lr

.LHerrGot:

	add				r3, pc, r3

	vsub.f64		d0, d0				@ D0 = 0.0
	ldr				r2, .L2020
	vmov.f64		d1, d0				@ D1 = 0.0
	ldr				r3, [r3, r2]
	cmp				r1, 0
	vmov.f64		d2, d0				@ D2 = 0.0
	moveq			r1, r3
	cmpne			r1, r3
	vldmia.f64		r1, {d5-d7}

	beq				.LStore

	vadd.f64		d4, d5, d6
	vadd.f64		d4, d7
	vcmp.f64		d4, #0
	vmrs			APSR_nzcv, FPSCR

	adreq			r2, .LThird
	vldrne.64		d7, .LOne
	vldmiaeq.f64	r2, {d5-d7}

	vdivne.f64		d5, d5, d4
	vdivne.f64		d6, d6, d4

	vsub.f64		d7, d7, d6
	vsub.f64		d7, d7, d5

.LStore:

	@ D5 = red, D6 = green, D7 = blue
	@ D0 - D2 = 0.0
	@ R0 = output

	vmov.f64		d4, d5
	mov				r1, r0			@ R1 = address of output
	vmov.f64		d3, d5

	@ Black is all zeros in D0 - D2, plus D3-D5 = red

	vstmia.f64		r1!, {d0-D5}	@ Black and red stored.

	vmov.f64		d5, d6
	vmov.f64		d4, d6
	vmov.f64		d3, d0

	vstmia.f64		r1!, {d4-d6}	@ Green stored.

	vmov.f64		d6, d7
	vmov.f64		d5, d7
	vmov.f64		d4, d0

	vstmia.f64		r1!, {d5-d7}	@ Blue stored.

	vmov.f64		d5, d0

	vstmia.f64		r1!, {d0-d5}	@ Yellow and magenta stored.
	vstmia.f64		r1, {d0-d5}		@ Cyan and white stored.

	bx				lr

	.align	3

.LThird:

	.word	0x55555555, 0x3fd55555
	.word	0x55555555, 0x3fd55555

.LOne:

	.word	0, 0x3ff00000

.LGot:

	.word	_GLOBAL_OFFSET_TABLE_-(.LHerrGot+8)

.L2020:

	.word	KOLIBA_Rec2020(GOT)

	.size	KOLIBA_ConvertGrayToFlut, .-KOLIBA_ConvertGrayToFlut
