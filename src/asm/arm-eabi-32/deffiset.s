/*

	deffiset.s - ARM EABI 32-bit version

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
	@		R0 = address of output EFFILUT
	@		D0 = cmyk efficacy
	@		D1 = rgbw efficacy
	@
	@	Set cyan, magenta, yellow, black to cmyk efficacy,
	@	the rest to rgbw efficacy.

	.arch	 armv6
	.text
	.align	2
	.global	KOLIBA_SetDualEfficacies
	.syntax unified
	.arm
	.fpu 	vfp
	.type	KOLIBA_SetDualEfficacies, %function

KOLIBA_SetDualEfficacies:

	@ Do this only if R0 is not NULL.

	subs			r1, r0, 0

	vmovne.f64		d2, d1		@ Green, red
	vmovne.f64		d3, d0		@ Cyan, magenta

	vstmiane.f64	r1!, {d0-d3}	@ black, blue, green, cyan
	vstmiane.f64	r1!, {d2-d3}	@ red, magenta
	vstmiane.f64	r1,  {d0-d1}	@ yellow, white

	bx				lr


	.size	KOLIBA_SetDualEfficacies, .-KOLIBA_SetDualEfficacies
