/*

	slt2plut.s - ARM EABI 32-bit version

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
	@		R0 = address of pLut
	@		R1 = address of (const) sLut
	@
	@	The pLut and the sLut may overlap, so we move the sLut
	@	to the stack before we copy it to the pLut.
	@
	@	For speed, we will push it all backwards, so when we pop it,
	@	we can just store it in the pLut rapidly.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertSlutToPlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertSlutToPlut, %function

KOLIBA_ConvertSlutToPlut:

	@ If sLut or pLut is zero, return 0.

	cmp		r1, 0
	moveq	r0, 0
	cmpne	r0, 0
	bxeq	lr

	@ Else ...

	push	{r4, r5, r6, r7, r8, r9, r10, lr}

	/*
		KOLIBA_PLUT offsets are (in bytes from the start of the structure):

			black		=	  0
			white		=	 24
			red			=	 48
			green		=	 72
			blue		=	 96
			cyan		=	120
			magenta		=	144
			yellow		=	168
			divisor		=	192
			efficacy	=	200

		KOLIBA_SLUT offsets are:

			black		=	  0
			blue		=	 24
			green		=	 48
			cyan		=	 72
			red			=	 96
			magenta		=	120
			yellow		=	144
			white		=	168

		We shall push the contents of sLut in reverse pLut order
		(not counting divisor and efficacy, which we will set to
		double 1.0 after the move).
	*/

	.equ	black,		0
	.equ	blue,		black+24
	.equ	green,		blue+24
	.equ	cyan,		green+24
	.equ	red,		cyan+24
	.equ	magenta,	red+24
	.equ	yellow,		magenta+24
	.equ	white,		yellow+24

	@ We push colors in reverse pLut order, though we do not
	@ need to push the last (black) color, just to pop it again.

	mov		r9, 7
	adr		r10, .sLutOffsets

.Lpush:

	ldrb	r8, [r9, r10]	@ Get the offset of the next color within sLut structure.
	add		r8, r8, r1		@ Add it to the address of the sLut argument.
	ldmia	r8, {r2, r3, r4, r5, r6, r7}
	subs	r9, r9, 1
	pushpl	{r2, r3, r4, r5, r6, r7}	@ Do not waste time pushing the last one.

	bpl		.Lpush

	@ Now pop them in straight pLut order and store them
	@ in the output pLut structure. However, the black
	@ values have not been pushed because they already
	@ are in the registers. So we do not need to pop them.
	@
	@ The conditional popPL will take care of it because
	@ the last subS resulted in a negative value (-1) and
	@ the flags still reflect that.

	mov		r9, 8
	mov		r8, r0

.Lpop:

	poppl	{r2, r3, r4, r5, r6, r7}	@ If "mi", values still in the registers.
	subs	r9, r9, 1
	stmia	r8!, {r2, r3, r4, r5, r6, r7}
	bne	.Lpop

	@ And finally, set pLut->divisor and pLut->efficacy to 1.0d.
	mov		r3, 0x3F000000
	mov		r2, 0
	add		r3, 0x00F00000

	@ At this point R8 is pointing at pLut->divisor already.

	strd	r2, r3, [r8], 8		@ divisor
	strd	r2, r3, [r8]		@ efficacy

	@ We are done, return.

	pop	{r4, r5, r6, r7, r8, r9, r10, pc}

	.align	2

.sLutOffsets:

	@ These are offsets within the sLut structure
	@ arranged in the pLut order.

	.byte	black, white, red, green, blue, cyan, magenta, yellow

	.size	KOLIBA_ConvertSlutToPlut, .-KOLIBA_ConvertSlutToPlut


