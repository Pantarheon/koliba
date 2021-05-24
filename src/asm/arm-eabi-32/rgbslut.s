/*

	rgbslut.s - ARM EABI 32-bit version

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
	@		R1 = address of (const) input RGB color
	@
	@	Used internally by libkoliba, hence the hidden attribute.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_CopyColorToSlutVertices
	.hidden	KOLIBA_CopyColorToSlutVertices	@ = __attribute__((visibility("hidden")))
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_CopyColorToSlutVertices, %function

KOLIBA_CopyColorToSlutVertices:

	@ Return NULL if either argument is NULL.

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Else, copy the RGB color to all eight vertices of the sLut.

	push		{r4, r5, r6, r7, lr}

	ldm			r1, {r2, r3, r4, r5, r6, r7}
	mov			r12, 8
	mov			r1, r0

.LLoop:

	subs		r12, r12, 1
	stmia		r1!, {r2, r3, r4, r5, r6, r7}
	bne			.LLoop

	pop			{r4, r5, r6, r7, pc}

	.size	KOLIBA_CopyColorToSlutVertices, .-KOLIBA_CopyColorToSlutVertices
