/*

	effilut.s - ARM EABI 32-bit version

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
	@		R2 = address of (const) eLut
	@		R3 = address of (const) modifier sLut

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ApplyEfficacies
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ApplyEfficacies, %function

	@ Negate the values of efficacies

KOLIBA_ApplyEfficacies:

	@ If either of the arguments is NULL, return NULL.

	cmp			r3, 0
	cmpne		r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Else, do the work.

	push		{r0, r4, r5, r6, r7, r8, r9, lr}

	push		{r0, r1, r2, r3}
	pop			{r4, r5, r6, r7}

	/*
		R4 = oLut
		R5 = iLut
		R6 = eLut
		R7 = mLut
	*/

	mov			r8, 0

.Loop:

	mov			r3, 3
	add			r0, r4, r8, lsl 3
	add			r1, r5, r8, lsl 3
	vldmia		r6!, {d0}
	add			r2, r7, r8, lsl 3
	add			r8, r8, r3

	bl			KOLIBA_Interpolate(PLT)

	cmp			r8, 8*3
	bne			.Loop

	pop			{r0, r4, r5, r6, r7, r8, r9, pc}

	.size	KOLIBA_ApplyEfficacies, .-KOLIBA_ApplyEfficacies
