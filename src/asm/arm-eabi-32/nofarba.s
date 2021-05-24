/*

	nofarba.s - ARM EABI 32-bit version

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
	@	We remove the farba, not to be confused with color from a sLut.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_DiscardSlutFarba
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_DiscardSlutFarba, %function

KOLIBA_DiscardSlutFarba:

	@ If either argument is NULL, return NULL.

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Else copy black to parts of the farba that default to 0,
	@ and white to parts of the farba that default to 1.
	@
	@ So, first copy black to {d0-d2} and white to {d4-d6}.
	add			r2, r1, 168
	vldm.f64	r1, {d0-d2}
	mov			r1, r0
	vldm.f64		r2, {d4-d6}

	@ black
	vstmia.f64	r1!, {d0-d2}

	@ blue
	vstmia.f64	r1!, {d0-d1}
	vstmia.f64	r1!, {d6}

	@ green
	vstmia.f64	r1!, {d0}
	vstmia.f64	r1!, {d5}
	vstmia.f64	r1!, {d2}

	@ cyan
	vstmia.f64	r1!, {d0}
	vstmia.f64	r1!, {d5-d6}

	@ red
	vstmia.f64	r1!, {d4}
	vstmia.f64	r1!, {d1-d2}

	@ magenta
	vstmia.f64	r1!, {d4}
	vstmia.f64	r1!, {d1}
	vstmia.f64	r1!, {d6}

	@ yellow
	vstmia.f64	r1!, {d4-d5}
	vstmia.f64	r1!, {d2}

	@ white
	vstmia.f64	r1, {d4-d6}
	bx	lr

	.size	KOLIBA_DiscardSlutFarba, .-KOLIBA_DiscardSlutFarba
