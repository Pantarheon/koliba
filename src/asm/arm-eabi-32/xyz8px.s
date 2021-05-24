/*

	xyz8px.s - ARM EABI 32-bit version

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
	@		  R0    = address of output RGBA8 pixel
	@		  R1    = address of (const) input XYZ pixel
	@		  R2    = address of (const) output conversion array, or NULL
	@
	@ On Exit:
	@
	@		 R0    = address of output RGBA8 pixel

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_XyzToRgba8Pixel
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_XyzToRgba8Pixel, %function

KOLIBA_XyzToRgba8Pixel:

	@ Load the address of our vars to R12

	adr					r12, .L255

	@ Load the XYZ doubles to {D0-D2}

	vldmia.f64			r1, {d0-d2}			@ D0 = x, D1 = y, D2 = z

	@ Load the double vars we need to {d3-d6}

	vldmia.f64			r12, {d3-d6}

	/*
		D3 = 255.0
		D4 = 0.5
		D5 = 254.5 / 255.0
		D6 < 0.5 / 255.0
	*/

	@ Process the red channel = x * 255

	vcmp.f64			d0, d6			@ x <= 0.5/255.0 ?
	vmrs				APSR_nzcv, FPSCR
	movle				r1, 0
	vcmpgt.f64			d0, d5			@ x >= 254.5/255.0 ?
	ble					.LGreen
	vmrs				APSR_nzcv, FPSCR
	vmovlt.f64			d7, d4
	vmlalt.f64			d7, d0, d3
	movge				r1, 255
	vcvtlt.u32.f64		s0, d7
	vmovlt				r1, s0

.LGreen:

	vcmp.f64			d1, d6			@ y <= 0.5/255.0 ?
	vmrs				APSR_nzcv, FPSCR
	movle				r3, 0
	vcmpgt.f64			d1, d5			@ y >= 254.5/255.0 ?
	ble					.LBlue
	vmrs				APSR_nzcv, FPSCR
	vmovlt.f64			d7, d4
	vmlalt.f64			d7, d1, d3
	movge				r3, 255
	vcvtlt.u32.f64		s0, d7
	vmovlt				r3, s0

.LBlue:

	vcmp.f64			d2, d6			@ z <= 0.5/255.0 ?
	vmrs				APSR_nzcv, FPSCR
	movle				r12, 0
	vcmpgt.f64			d2, d5			@ z >= 254.5/255.0 ?
	ble					.LStore
	vmrs				APSR_nzcv, FPSCR
	vmlalt.f64			d4, d2, d3
	movge				r12, 255
	vcvtlt.u32.f64		s0, d4
	vmovlt				r12, s0

.LStore:

	@ If we were passed a conversion array, use it.

	cmp					r2, 0

	ldrbne				r1, [r2, r1]
	ldrbne				r3, [r2, r3]
	orr					r3, r1, r3, lsl 8
	ldrbne				r12, [r2, r12]
	strh				r3, [r0]				@ Red and green channels
	strb				r12, [r0, 2]			@ Blue channel

	@ Hasta la Pasta!

	bx					lr

	.align 3

.L255:

	.word	0, 0x406FE000

.LHalf:

	.word	0, 0x3FE00000

.LOne:

	@ Actually, this is slightly less than 1.0,
	@ but the nearest integer of it multiplied
	@ by 255.0 is 255.

	.word	0xEFEFEFF0, 0x3FEFEFEF	@ = 254.5 / 255.0 = 0.998039

.LLittle:

	.word	0x10101010, 0x3F601010	@ < 0.5 / 255.0 = 0.00196078...

	.size	KOLIBA_XyzToRgba8Pixel, .-KOLIBA_XyzToRgba8Pixel
