/*

	msflt.s - ARM EABI 32-bit version

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
	@		R0 = address of output KOLIBA_MATRIX
	@		R1 = address of (const) input KOLIBA_FLUT

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertFlutToMatrix
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertFlutToMatrix, %function

KOLIBA_ConvertFlutToMatrix:

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	mov				r2, r0

	@ Matrix Red

	vldr.64			d0, [r1,24]			@ red.r
	vldr.64			d1, [r1,48]			@ green.r
	vldr.64			d2, [r1,72]			@ blue.r
	vldr.64			d3, [r1]			@ black.r

	@ Matrix Green

	vldr.64			d4, [r1,32]			@ red.g
	vldr.64			d5, [r1,56]			@ green.g
	vldr.64			d6, [r1,80]			@ blue.g
	vldr.64			d7, [r1,8]			@ black.g

	vstmia.f64		r2!, {d0-d7}

	@ Matrix Blue
	vldr.64			d0, [r1,40]			@ red.b
	vldr.64			d1, [r1,64]			@ green.b
	vldr.64			d2, [r1,88]		@ blue.b
	vldr.64			d3, [r1,16]		@ black.b

	vstmia.f64		r2, {d0-d3}
	bx				lr

	.size	KOLIBA_ConvertFlutToMatrix, .-KOLIBA_ConvertFlutToMatrix
