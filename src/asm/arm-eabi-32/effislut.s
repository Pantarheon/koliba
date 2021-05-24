/*

	effislut.s - ARM EABI 32-bit version

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
	@		D0 = efficacy

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.equ	slut, (3*8*8)

	.align	2
	.global	KOLIBA_SlutEfficacy
	.type	KOLIBA_SlutEfficacy, %function

KOLIBA_SlutEfficacy:

	@ Reject NULL pointers

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Set up local variables

	add			r1, slut
	vldr.f64	d1, .One
	mov			r3, 24
	ldr			r2, =((1<<5)|(1<<7)|(7<<10)|(1<<15)|(7<<17)|(7<<21))<<8
	vsub.f64	d1, d1, d0
	mov			r12, r0

.TheLoop:

	vldmdb.f64	r1!, {d2}
	lsls		r2, r2, 1
	vmul.f64	d2, d0, d2
	vaddcs.f64	d2, d1, d2
	subs		r3, r3, 1
	vstmdb.f64	sp!, {d2}		@ i.e., push it on the stack.
	bne			.TheLoop

	@ Copy the results from the stack to the output.

	mov			r3, 3

.CopyLoop:

	vldmia.f64	sp!, {d0-d7}	@ i.e., pop it off the stack.
	subs		r3, r3, 1
	vstmia.f64	r12!, {d0-d7}
	bne			.CopyLoop

	@ Hasta la Pasta!

	bx			lr


	.align	3

.One:

	.double	1.0


	.size	KOLIBA_SlutEfficacy, .-KOLIBA_SlutEfficacy
