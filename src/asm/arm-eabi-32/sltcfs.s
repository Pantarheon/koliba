/*

	sltcfs.s - ARM EABI 32-bit version

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
	@		R0 = address of output sLut
	@		R1 = address of (const) input cFlt

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertColorFilterToSlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertColorFilterToSlut, %function

	@ Negate the values of efficacies

KOLIBA_ConvertColorFilterToSlut:

	@ If either of the arguments is NULL, return NULL.

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Else, do the work.

	vldmia.f64	r1, {d0-d3}
	vldr		d5, .LOne
	mov			r2, 0
	mov			r3, 0
	add			r1, r0, 3*8
	strd		r2, r3, [r0]
	strd		r2, r3, [r0, 8]
	strd		r2, r3, [r0, 16]	@ Black = {0, 0, 0}
	vstr.64		d5, [r1, 6*24]
	vmul.f64	d0, d3, d0			@ D0 = r * d + 0 * (1 - d)
	vstr.64		d5, [r1, 6*24+8]
	vmul.f64	d1, d3, d1			@ D1 = g * d + 0 * (1 - d)
	vstr.64		d5, [r1,6*24+16]	@ White = {1, 1, 1}
	vsub.f64	d5, d5, d3
	vmul.f64	d2, d3, d2			@ D2 = b * d + 0 * (1 - d)
	vadd.f64	d3, d5, d0			@ D3 = r * d + 1 * (1 - d)
	vadd.f64	d4, d5, d1			@ D4 = g * d + 1 * (1 - d)
	vadd.f64	d5, d5, d2			@ D5 = b * d + 1 * (1 - d)

	@ Blue = {r*d, g*d, b*d+1-d}
	vstmia.f64	r1!, {d0-d1}
	vstmia.f64	r1!, {d5}

	@ Green = {r*d, g*d+1-d, b*d}
	vstmia.f64	r1!, {d0}
	vstmia.f64	r1!, {d4}
	vstmia.f64	r1!, {d2}

	@ Cyan = {r*d, g*d+1-d, b*d+1-d}
	vstmia.f64	r1!, {d0}
	vstmia.f64	r1!, {d4-d5}

	@ Red = {r*d+1-d, g*d, b*d}
	vstmia.f64	r1!, {d3}
	vstmia.f64	r1!, {d1-d2}

	@ Magenta = {r*d+1-d, g*d, b*d+1-d}
	vstmia.f64	r1!, {d3}
	vstmia.f64	r1!, {d1}
	vstmia.f64	r1!, {d5}

	@ Yellow = {r*d+1-d, d*s+1-d, b*d}
	vmov.f64	d5, d2
	vstmia.f64	r1!, {d3-d5}

	bx			lr

.LOne:

	.word		0, 0x3FF00000

	.size	KOLIBA_ConvertColorFilterToSlut, .-KOLIBA_ConvertColorFilterToSlut
