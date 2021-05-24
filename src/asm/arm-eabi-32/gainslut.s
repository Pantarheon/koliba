/*

	gainslut.s - ARM EABI 32-bit version

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
	@		  R0    = address of input/output SLUT
	@		  R1    = flags (byte)
	@		  D0    = gain
	@
	@ On Exit:
	@
	@		 R0    = unchanged

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	3
	.global	KOLIBA_ApplyGainToSlutVertices
	.type	KOLIBA_ApplyGainToSlutVertices, %function

KOLIBA_ApplyGainToSlutVertices:

	@ If no SLUT, no service. Else, prepare local variables.

	cmp			r0, 0
	mov			r2, r0		@ R2 = SLUT
	mov			r3, 8		@ R3 = counter
	bxeq		lr

.TheLoop:

	lsrs		r1, r1, 1	@ is a flag set?
	bcc			.Next		@ no, continue.

	@ Flag is set, apply lift.

	vldmia.f64	r2, {d1-d3}

	vmul.f64	d1, d0, d1
	vmul.f64	d2, d0, d2
	vmul.f64	d3, d0, d3

	vstmia.f64	r2, {d1-d3}

.Next:

	subs		r3, r3, 1
	add			r2, r2, 3*8
	bne			.TheLoop

	@ Hasta la Pasta!

	bx			lr

	.size	KOLIBA_ApplyGainToSlutVertices, .-KOLIBA_ApplyGainToSlutVertices
