/*

	externpx.s - ARM EABI 32-bit version

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
	@		R0 = address of output pixel
	@		R1 = address of (const) input pixel
	@		R2 = address of (const) KOLIBA_FFLUT
	@		R3 = n
	@		+ 3 more on the stack to pass to KOLIBA_ExternalXyz
	@		+ transformin and transformout (they ma be NULL) further down the stack
	@
	@ On Exit:
	@
	@		R0 = address output pixel or NULL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ExternalPixel
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ExternalPixel, %function

KOLIBA_ExternalPixel:

	@ Reject NULL pointers

	cmp				r1, 0
	movne			r12, sp
	moveq			r0, 0
	cmpne			r0, 0
	pushne			{r4, r5, r6, r7, r8, lr}
	bxeq			lr

	@ Convert pixel to XYZ, call KOLIBA_ExternalXyz
	@ and convert the result back to pixel.

	ldmia			r12, {r4-r8}	@ We need to move the stack parameters to a new TOS, and more.

	@ R4-R6 = parameters to pass on
	@ R7 = transformin, or NULL
	@ R8 = transformout, or NULL

	cmp				r7, 0					@ Do we have a tranformin? EQ = no, NE = yes

	vldmiaeq.f64	r1, {s6-s8}
	vcvteq.f64.f32	d0, s6
	vcvteq.f64.f32	d1, s7
	subne			sp, sp, 3*8
	vcvteq.f64.f32	d2, s8
	vpusheq.f64		{d0, d1, d2}
	push			{r4, r5, r6, lr}		@ We push LR, just to keep the stack 8-byte aligned.
	mov				r4, r0					@ We no longer need R4 for transfer, so we save R0 in it.
	add				r0, sp, 4*4				@ Address of the pixel converted to XYZ.
	push			{r2-r3}
	blxne			r7
	add				r1, sp, 6*4				@ Address of the pixel converted to XYZ.
	pop				{r2-r3}
	cmp				r0, r1
	addne			sp, sp, 4*4+3*8
	movne			r0, 0
	popne			{r4, r5, r6, r7, r8, pc}

	bl				KOLIBA_ExternalXyz(PLT)

	cmp				r0, 0					@ If it returned NULL, return the NULL as well.
	addne			sp, sp, 4*4				@ Clear off the 3 parameters we pushed, plus the extra 4 bytes.
	addeq			sp, sp, 4*4+3*8
	popeq			{r4, r5, r6, r7, r8, pc}

	cmp				r8, 0
	mov				r0, r4

	vldmiaeq.f64	sp, {d0, d1, d2}
	vcvteq.f32.f64	s6, d0
	vcvteq.f32.f64	s7, d1
	movne			r1, sp
	vcvteq.f32.f64	s8, d2
	vstmiaeq.f32	r4, {s6-s8}
	blxne			r8
	cmp				r0, r4
	add				sp, sp, 3*8
	movne			r0, 0

	pop				{r4, r5, r6, r7, r8, pc}

	.size	KOLIBA_ExternalPixel, .-KOLIBA_ExternalPixel
