/*

	flbinflut.s - ARM EABI 32-bit version

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
	@		R0 = address of fLut
	@		R1 = address of (const) flbindex
	@		R2 = address of (const) doubles

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertFlbindexToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertFlbindexToFlut, %function

KOLIBA_ConvertFlbindexToFlut:

	@ Reject NULL pointers

	cmp			r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Do the job

	mov			r12, 3*8					@ That many doubles in a FLUT.
	push		{r0, r4, r5, lr}

.Loop:

	subs		r12, r12, 1
	ldrbne		r3, [r1], 1					@ R3 = *flbindex, R1++;
	popeq		{r0, r4, r5, pc}			@ Done, hasta la pasta!

	add			r3, r2, r3, lsl 3			@ R3 = address of doubles[*flindex];
	ldrd		r4, [r3]					@ R4,R5 = doubles[*flindex];
	strd		r4, [r0], 8					@ R0++ = R4, R5;
	b			.Loop

	.size	KOLIBA_ConvertFlbindexToFlut, .-KOLIBA_ConvertFlbindexToFlut
