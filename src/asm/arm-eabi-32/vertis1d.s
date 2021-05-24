/*

	vertis1d.s - ARM EABI 32-bit version

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
	@		  R0    = address of (const) KOLIBA_VERTICES
	@
	@ On Exit:
	@
	@		 R0    = 0 or 1

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.equ	black,	0
	.equ	blue,	(black+4)
	.equ	green,	(blue+4)
	.equ	cyan,	(green+4)
	.equ	red,	(cyan+4)
	.equ	magenta,	(red+4)
	.equ	yellow,	(magenta+4)
	.equ	white,	(yellow+4)

	.equ	r,	0
	.equ	g,	(r+8)
	.equ	b,	(g+8)

	.align	2
	.global	KOLIBA_VerticesIs1D
	.type	KOLIBA_VerticesIs1D, %function

KOLIBA_VerticesIs1D:

	@ If there are no vertices, it is not 3D...

	cmp			r0, 0
	mov			r12, r0
	bxeq		lr

	@ If any test fails, the answer is no, so return 0.
	@ Only if all tests pass, return 1.

	@ Are black and white values comparable with blue values?

	ldr			r1, [r0,black]		@ R1 = black
	ldr			r2, [r0, white]		@ R2 = white
	ldr			r3, [r0, blue]		@ R3 = blue

	@ Assume it is not 3D and only change
	@ if shown otherwise.

	mov			r0, 0

	vldmia.f64	r1, {d0-d2}			@ D0-D2 = black
	vldmia.f64	r3, {d6-d7}			@ D6 = blue.r, D7 = blue.g
	vldmia.f64	r2, {d3-d5}			@ D3-D5 = white

	vcmp.f64	d0, d6				@ black.r ?= blue.r
	ldr			r1, [r12, red]		@ R1 = red
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d1, d7
	ldr			r2, [r12, green]	@ R2 = green
	bxne		lr					@ no, return 0.

	@ yes, continue. black.g ?= blue.g
	vmrs		APSR_nzcv, FPSCR
	vldr.f64	d6, [r3,b]			@ D6 = blue.b
	bxne		lr					@ no, return 0.

	@ yes, continue.
	vcmp.f64	d5, d6				@ white.b ?= blue.b
	vmrs		APSR_nzcv, FPSCR
	vldmia.f64	r1, {d6-d7}			@ D6 = red.r, D7 = red.g
	bxne		lr					@ no, return 0

	@ yes, continue.
	vcmp.f64	d3, d6				@ white.r ?= red.r
	vldr.f64	d6, [r1,b]			@ D6 = red.b
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d1, d7
	bxne		lr					@ no, return 0.

	@ yes, continue. black.g ?= red.g
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d2, d6
	bxne		lr					@ no, return 0.

	@ yes, continue. black.b ?= red.b
	vldmia.f64	r2, {d6, d7}		@ D6 = green.r, D7 = green.g
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d0, d6
	bxne		lr					@ no, return 0.

	@ yes, continue. black.r ?= green.r
	vldr.f64	d6, [r2,b]			@ D6 = green.b
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d4, d7
	bxne		lr					@ no, return 0.

	@ yes, continue. white.g ?= green.g
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d2, d6
	ldr			r3, [r12,cyan]
	bxne		lr					@ no, return 0.

	@ yes, continue. black.b ?= green.b
	vldmia.f64	r3, {d6-d7}			@ D6 = cyan.r, D7 = cyan.g
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d0, d6
	bxne		lr					@ no, return 0.

	@ yes, continue. black.r ?= cyan.r
	vldr.f64	d6, [r3,b]			@ D6 = cyan.b
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d4, d7
	bxne		lr					@ no, return 0.

	@ yes, continue. white.g ?= cyan.g
	vmrs		APSR_nzcv, FPSCR
	ldr			r1, [r12,magenta]
	vcmp.f64	d5, d6
	vldmia.f64	r1, {d6-d7}			@ D6 = magenta.r, D7 = magenta.g
	bxne		lr					@ no, return 0.

	@ yes, continue. white.b ?= cyan.b
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d3, d6
	vldr.f64	d6, [r1,b]			@ D6 = magenta.b
	bxne		lr					@ no, return 0.

	@ yes, continue. white.r ?= magenta.r
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d1, d7
	bxne		lr					@ no, return 0.

	@ yes, continue. black.g ?= magenta.g
	ldr			r2, [r12,yellow]
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d5, d6
	bxne		lr					@ no, return 0

	@ yes, continue. white.b ?= magenta.b
	vldmia.f64	r2, {d5-d7}			@ D5 = yellow.r, D6 = yellow.g, D7 = yellow.b
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d3, d5
	bxne		lr					@ no, return 0.

	@ yes, continue. white.r ?= yellow.r
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d4, d6
	bxne		lr					@ no, return 0.

	@ yes, continue. white.g ?= yellow.g
	vmrs		APSR_nzcv, FPSCR
	vcmp.f64	d2, d7
	bxne		lr					@ no, return 0.

	@ yes, finalize. black.b ?= yellow.b
	vmrs		APSR_nzcv, FPSCR
	moveq		r0, 1				@ return (yes) ? 1 : 0
	bx			lr

	.size	KOLIBA_VerticesIs1D, .-KOLIBA_VerticesIs1D
