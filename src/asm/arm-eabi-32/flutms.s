/*

	flutms.s - ARM EABI 32-bit version

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
	@		R0 = address of output fLut
	@		R1 = address of (const) input matrix

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertMatrixToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertMatrixToFlut, %function

KOLIBA_ConvertMatrixToFlut:

	@ Check for NULL pointers

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Do the conversion

	vpush.f64	{d8-d11}

	vldr.64		d0, [r1, 3*8]			@ D0  = fLut->KR = mat->red.o
	vldr.64		d1, [r1, 7*8]			@ D1  = fLut->KG = mat->green.o
	vldr.64		d2, [r1, 11*8]			@ D2  = fLut->KB = mat->blue.o

	vldr.64		d3, [r1]				@ D3  = fLut->RR = mat->red.r
	vldr.64		d4, [r1, 4*8]			@ D4  = fLut->RG = mat->green.r
	vldr.64		d5, [r1, 8*8]			@ D5  = fLut->RB = mat->blue.r

	vldr.64		d6, [r1, 8]				@ D6  = fLut->GR = mat->red.g
	vldr.64		d7,	[r1, 5*8]			@ D7  = fLut->GG = mat->green.g
	vldr.64		d8, [r1, 9*8]			@ D8  = fLut->GB = mat->blue.g

	vldr.64		d9, [r1, 2*8]			@ D9  = fLut->BR = mat->red.b
	vldr.64		d10, [r1, 6*8]			@ D10 = fLut->BG = mat->green.b
	vldr.64		d11, [r1, 10*8]			@ D11 = fLut->BB = mat->blue.b

	ldr			r2, .LZeroes
	mov			r1, r0

.LZeros:

	add			r2, pc, r2

	vstmia		r1!, {d0-d11}
	vldmia		r2, {d0-d11}
	vstmia		r1, {d0-d11}

	vpop.f64	{d8-d11}
	bx			lr

	.align	2

.LZeroes:

	.word	KOLIBA_Zeroes-(.LZeros+8)
	.size	KOLIBA_ConvertMatrixToFlut, .-KOLIBA_ConvertMatrixToFlut
	.hidden	KOLIBA_Zeroes
