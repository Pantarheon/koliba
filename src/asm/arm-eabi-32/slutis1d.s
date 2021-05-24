/*

	slutis1d.s - ARM EABI 32-bit version

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
	@		  R0   = address of (const) input sLut
	@
	@ On Exit:
	@
	@		  R0   = 1 if sLut is 1D, 0 if not or if NULL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_SlutIs1D
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_SlutIs1D, %function

KOLIBA_SlutIs1D:

	@ If it does not exist, it is not 1D (or anything else).

	cmp				r0, 0
	mov				r2, r0
	bxeq			lr

	@ Load black[r,g,b] to [D0-D2]

	add				r1, r2, 3*8			@ R1 = address of blue.r
	vldmia.f64		r2, {d0-d2}			@ D0 = black.r, D1 = black.g, D2 = black.b
	mov				r0, 0				@ The odds are higher of the answer being a no.

	@ Load blue[r, g] and green[r, b] and compare, etc.

	vldmia.f64		r1, {d3-d4}			@ D3 = blue.r, D4 = blue.g
	vldr.f64		d5, [r1,3*8]		@ D5 = green.r
	vldr.f64		d6, [r1,5*8]		@ D6 = green.b

	vcmp.f64		d0, d3				@ black.r ?= blue.r
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d0, d5				@ black.r ?= green.r
	bxne			lr					@ black.r <> blue.r
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d4				@ black.g ?= blue.g
	add				r1, r2, 13*8		@ R1 = address of red.g
	vldr.f64		d5, [r2,9*8]		@ D5 = cyan.r
	bxne			lr					@ black.r <> green.r
	vldmiaeq.f64	r1!, {d3-d4}		@ D3 = red.g, D4 = red.b, R1 = address of magenta
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d0, d5				@ black.r ?= cyan.r
	vldr.f64		d5, [r1,8]			@ D5 = magenta.g
	bxne			lr					@ black.g <> blue.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d3				@ black.g ?= red.g
	vldr.f64		d3, [r1,5*8]		@ D3 = yellow.b
	bxne			lr					@ black.r <> cyan.r
	add				r1, r1, 6*8			@ R1 = address of white
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d5				@ black.g ?= magenta.g
	add				r3, r2, 10*8		@ R3 = address of cyan.g
	vldr.f64		d5, [r2,5*8]		@ D5 = blue.b
	bxne			lr					@ black.g <> red.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d6				@ black.b ?= green.b
	vldmia.f64		r3!, {d6-d7}		@ D6 = cyan.g, D7 = cyan.b, R3 = address of red.r
	bxne			lr					@ black.g <> magenta.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d4				@ black.b ?= red.b
	vldr.f64		d4, [r3,5*8]		@ D4 = magenta.b
	bxne			lr					@ black.b <> green.b
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d3				@ black.b ?= yellow.b
	vldr.f64		d3, [r2,7*8]		@ D3 = green.g
	bxne			lr					@ black.b <> red.b

	vldmia.f64		r1, {d0-d2}			@ D0 = white.r, D1 = white.g, D2 = white.b
	vmrs			APSR_nzcv, FPSCR
	add				r1, r3, 6*8			@ R1 = address of yellow.r
	vcmp.f64		d2, d5				@ white.b ?= blue.b
	bxne			lr					@ black.b <> yellow.b
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d4				@ white.b ?= magenta.b
	vldmia.f64		r1, {d4-d5}			@ D4 = yellow.r, D5 = yellow.g
	bxne			lr					@ white.b <> blue.b
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d7				@ white.b ?= cyan.b
	vldr.f64		d7, [r3,3*8]		@ D7 = magenta.r
	bxne			lr					@ white.b <> magenta.b
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d3				@ white.g ?= green.g
	vldr.f64		d3, [r3]			@ D3 = red.r
	bxne			lr					@ white.b <> cyan.b
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d6				@ white.g ?= cyan.g
	bxne			lr					@ white.g <> green.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d1, d5				@ white.g ?= yellow.g
	bxne			lr					@ white.g <> cyan.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d0, d3				@ white.r ?= red.r
	bxne			lr					@ white.g <> yellow.g
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d0, d7				@ white.r ?= magenta.r
	bxne			lr					@ white.r <> red.r
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d0, d4				@ white.r ?= yellow.r
	bxne			lr					@ white.r <> magenta.r
	vmrs			APSR_nzcv, FPSCR

	moveq			r0, 1				@ EQ = white.r = yellow.r, else not
	bx				lr

	.size	KOLIBA_SlutIs1D, .-KOLIBA_SlutIs1D
