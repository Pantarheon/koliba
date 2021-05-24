/*

	rgb2flut.s - ARM EABI 32-bit version

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
	@		R1 = address of (const) color (KOLIBA_RGB)
	@
	@ On Exit:
	@
	@		R0 = address of fLut, or NULL
	@
	@	We only return NULL if fLut is NULL. If color
	@	is NULL, we just use black.
	@
	@	A color fLut simply has the color in its black
	@	vertex. The rest of its vertices are zeroes.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertRgbToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertRgbToFlut, %function

KOLIBA_ConvertRgbToFlut:

	cmp			r0, 0
	ldr			r2, .LZeroes
	bxeq		lr

.LZeros:

	add			r2, pc, r2			@ R2 = address of KOLIBA_Zeroes

	cmp			r1, 0
	mov			r3, r0
	moveq		r1, r2

	vldmia.f64	r2, {d3-d7}
	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r3!, {d0-d7}

	vldmia.f64	r2, {d0-d2}
	vstmia.f64	r3!, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			lr



	.align	2

.LZeroes:

	.word	KOLIBA_Zeroes-(.LZeros+8)

	.size	KOLIBA_ConvertRgbToFlut, .-KOLIBA_ConvertRgbToFlut
	.hidden	KOLIBA_Zeroes
