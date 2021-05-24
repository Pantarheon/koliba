/*

	erythropia.s - ARM EABI 32-bit version

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
	@		R1 = address of (const) input sLut
	@
	@	The two pointers may be the same. If they are not,
	@	it is still safe for the two sLuts to overlap, whether
	@	fully or partially.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ApplyErythropy
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ApplyErythropy, %function

KOLIBA_ApplyErythropy:

	@ If either pointer is NULL, return NULL.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	vpush.f64		{d8-d11}

	/*
		Koliba sLut offsets are:

			black		=	  0
			blue		=	 24
			green		=	 48
			cyan		=	 72
			red			=	 96
			magenta		=	120
			yellow		=	144
			white		=	168
	*/

	.equ	black,		0
	.equ	blue,		black+24
	.equ	green,		blue+24
	.equ	cyan,		green+24
	.equ	red,		cyan+24
	.equ	magenta,	red+24
	.equ	yellow,		magenta+24
	.equ	white,		yellow+24

	add				r2, r1, red
	cmp				r0, r1
	vldmia			r2, {d9-d11}		@ D9 = r, D10 = g, D11 = b

	@ If the two sLuts are at the same address, no need to
	@ copy black, red and white to them. Otherwise, we have to.

	mov				r3, 1
	addne			r2, r1, white		@ R2 = address of input.white
	vmov.u32		s16, r3
	vldmiane		r1, {d0-d2}			@ Black.
	vcvt.f64.u32	d8, s16				@ D8 = 1.0
	vldmiane		r2, {d3-d5}			@ White.
	vsub.f64		d6, d8, d9			@ D6 = c
	addne			r2, r0, white		@ R2 = address of output.white
	vsub.f64		d7, d8, d10			@ D7 = m
	vstmiane		r0, {d0-d2}
	vsub.f64		d8, d8, d11			@ D8 = y
	vstmiane		r2, {d3-d5}

	vmov.f64		d0, d10				@ blue.r    = g
	vmov.f64		d1, d11				@ blue.g    = b
	vmov.f64		d2, d9				@ blue.b    = r

	add				r2, r0, blue		@ R2 = address of output.blue

	vmov.f64		d3, d11				@ green.r   = b
	vmov.f64		d4, d9				@ green.g   = r
	vmov.f64		d5, d10				@ green.b   = g

	vstmia			r2!, {d0-d8}		@ copy blue, green and cyan to output

	vmov.f64		d0, d8				@ magenta.r = y
	vmov.f64		d1, d6				@ magenta.g = c
	vmov.f64		d2, d7				@ magenta.b = m

	vstmiane		r2!, {d9-d11}		@ if needed, copy red to output
	addeq			r2, r0, magenta		@ else set R2 to address of output.magenta

	vmov.f64		d3, d7				@ yellow.r  = m
	vmov.f64		d4, d8				@ yellow.g  = y
	vmov.f64		d5, d6				@ yellow.b  = c

	vstmia			r2, {d0-d5}			@ copy magenta and yellow to output

	vpop.f64		{d8-d11}
	bx				lr

	.size	KOLIBA_ApplyErythropy, .-KOLIBA_ApplyErythropy
