/*

	pltreset.s - ARM EABI 32-bit version

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
	@		  R0    = address of output KOLIBA_PALETTE
	@
	@ On Exit:
	@
	@		 R0    = same
	@
	@	The KOLIBA_PALETTE consists of eight KOLIBA_PIGMENTs,
	@	followed by a double efficacy, follow by the erythropy byte.
	@	green at bit 16, blue at bit 8. Each PIGMENT consists of
	@	four doubles (r, g, b, efficacy).

	.equ	PIGMENT,	(4*8)
	.equ	PALETTE,	(8*PIGMENT+8+1)
	.equ	black,	0
	.equ	white,	(black+PIGMENT)
	.equ	red,	(white+PIGMENT)
	.equ	green,	(red+PIGMENT)
	.equ	blue,	(green+PIGMENT)
	.equ	cyan,	(blue+PIGMENT)
	.equ	magenta,	(cyan+PIGMENT)
	.equ	yellow,	(magenta+PIGMENT)

	.text
	.arch	armv6
	.syntax	unified
	.arm
	.fpu	vfp

	.align	3

.GetPaletteDefaults:

	ldr			r3, .Got
	ldr			r2, .IdPal

.HerrGot:

	add			r3, pc, r3
	ldr			r2, [r3, r2]

	bx			lr

.Got:

	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IdPal:

	.word	KOLIBA_IdentityPalette(GOT)

	.align	3
	.global	KOLIBA_ResetPalette
	.type	KOLIBA_ResetPalette, %function

KOLIBA_ResetPalette:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	mov			r3, r0

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}

	vldr.f64	d0, [r2]
	ldrb		r12, [r2,8]
	vstr.f64	d0, [r3]
	strb		r12, [r3, 8]

	bx			r1

	.size	KOLIBA_ResetPalette, .-KOLIBA_ResetPalette

	.align	3
	.global	KOLIBA_ResetPaletteBlack
	.type	KOLIBA_ResetPaletteBlack, %function

KOLIBA_ResetPaletteBlack:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r0, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteBlack, .-KOLIBA_ResetPaletteBlack

	.align	3
	.global	KOLIBA_ResetPaletteWhite
	.type	KOLIBA_ResetPaletteWhite, %function

KOLIBA_ResetPaletteWhite:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, white
	add			r3, r0, white

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteWhite, .-KOLIBA_ResetPaletteWhite

	.align	3
	.global	KOLIBA_ResetPaletteRed
	.type	KOLIBA_ResetPaletteRed, %function

KOLIBA_ResetPaletteRed:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, red
	add			r3, r0, red

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteRed, .-KOLIBA_ResetPaletteRed

	.align	3
	.global	KOLIBA_ResetPaletteGreen
	.type	KOLIBA_ResetPaletteGreen, %function

KOLIBA_ResetPaletteGreen:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, green
	add			r3, r0, green

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteGreen, .-KOLIBA_ResetPaletteGreen

	.align	3
	.global	KOLIBA_ResetPaletteBlue
	.type	KOLIBA_ResetPaletteBlue, %function

KOLIBA_ResetPaletteBlue:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, blue
	add			r3, r0, blue

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteBlue, .-KOLIBA_ResetPaletteBlue

	.align	3
	.global	KOLIBA_ResetPaletteCyan
	.type	KOLIBA_ResetPaletteCyan, %function

KOLIBA_ResetPaletteCyan:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, cyan
	add			r3, r0, cyan

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteCyan, .-KOLIBA_ResetPaletteCyan

	.align	3
	.global	KOLIBA_ResetPaletteMagenta
	.type	KOLIBA_ResetPaletteMagenta, %function

KOLIBA_ResetPaletteMagenta:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, magenta
	add			r3, r0, magenta

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteMagenta, .-KOLIBA_ResetPaletteMagenta

	.align	3
	.global	KOLIBA_ResetPaletteYellow
	.type	KOLIBA_ResetPaletteYellow, %function

KOLIBA_ResetPaletteYellow:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr
	bl			.GetPaletteDefaults

	add			r2, r2, yellow
	add			r3, r0, yellow

	vldmia.f64	r2, {d0-d7}
	vstmia.f64	r3, {d0-d7}

	bx			r1

	.size	KOLIBA_ResetPaletteYellow, .-KOLIBA_ResetPaletteYellow
