/*

	px8xyz.s - ARM EABI 32-bit version

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
	@		  R0    = address of output xyz pixel
	@		  R1    = address of (const) input RGBA8 pixel
	@		  R2    = address of (const) doubles array
	@
	@ On Exit:
	@
	@		 R0    = address output xyz pixel
	@
	@	In RGBA8 LSB first, red starts at bit 0,
	@	green at bit 8, blue at bit 16.
	.equ	RBITS, 0
	.equ	GBITS, 8
	.equ	BBITS, 16

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Rgba8PixelToXyz
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Rgba8PixelToXyz, %function

KOLIBA_Rgba8PixelToXyz:

	adr			r12, .LConv
	cmp			r2, 0				@ Do we have a conversion array?
	ldmiaeq		r12, {r2-r3}
	ldr			r1, [r1]			@ R1 = RGBA pixel

.LHerrGot:

	addeq		r3, pc, r3
	uxtb		r12, r1, ror BBITS	@ R12 = blue
	ldreq		r2, [r3, r2]
	uxtb		r3, r1, ror GBITS	@ R3  = green
	add			r12, r2, r12, lsl 3
	uxtb		r1, r1, ror RBITS	@ R1  = red
	add			r3, r2, r3, lsl 3
	vldr.f64	d2, [r12]
	add			r1, r2, r1, lsl 3
	vldr.f64	d1, [r3]
	vldr.f64	d0, [r1]
	vstmia.f64	r0, {d0-d2}

	@ Hasta la Pasta!

	bx			lr


	.align	2

.LConv:

	.word	KOLIBA_ByteDiv255(GOT)

.LGot:

	.word	_GLOBAL_OFFSET_TABLE_-(.LHerrGot+8)

	.size	KOLIBA_Rgba8PixelToXyz, .-KOLIBA_Rgba8PixelToXyz
