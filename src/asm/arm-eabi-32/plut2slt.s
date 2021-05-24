/*

	plut2slt.s - ARM EABI 32-bit version

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
	@		R0 = address of sLut
	@		R1 = address of (const) pLut
	@
	@	The pLut and the sLut may overlap, so we move the pLut
	@	to the stack before we copy it to the sLut.
	@
	@	If pLut->divisor is less than 1, we return NULL;
	@	if it equals 1, we just copy the raw data to the stack;
	@	if it is greater than 1, we divide all the doubles by it.
	@
	@	Then we call the KOLIBA_Interpolate routine, which will
	@	apply the efficacy and copy it all from the stack to
	@	the output (*R0).

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertPlutToSlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertPlutToSlut, %function

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
*/

	@ Define offsets within the pLut structure

	.equ	black,		0
	.equ	white,		black+24
	.equ	red,		white+24
	.equ	green,		red+24
	.equ	blue,		green+24
	.equ	cyan,		blue+24
	.equ	magenta,	cyan+24
	.equ	yellow,		magenta+24

	.equ	divisor,	yellow+24
	.equ	efficacy,	divisor+8

KOLIBA_ConvertPlutToSlut:

	@ If sLut or pLut is zero, return 0.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	mov				r2, 1
	bxeq			lr

	@ Also return 0, if pLut->divisor is less than 1, an illegal value.

	vmov			s2, r2
	vldr			d0, [r1, divisor]	@ D0 = pLut->divisor
	vcvt.f64.u32	d1, s2				@ D1 = 1.0

	vcmp.f64		d0, d1
	vmrs			APSR_nzcv, fpscr	@ Copy FPU flags to CPU flags

	movlt			r0, 0				@ if D0 < D1 ...
	bxlt			lr					@ ... return 0 (NULL).

	@ Else, we have some work to do

	push			{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov				fp, sp
	sub				sp, sp, divisor+4	@ The +4 aligns it at an 8-byte boundary
	mov				r9, 7
	mov				r12, sp
	adr				r10, .pLutOffsets

	@ If pLut->divisor == 1.0, move raw data to the stack.
	@ This is probably the most common case, so we should only branch rarely.

	bne				.LDivide

.LCopy:

	ldrb			r8, [r9, r10]	@ Get offset of next color within pLut structure.
	add				r8, r8, r1		@ Add it to the address of the pLut argument.
	ldmia			r8, {r2, r3, r4, r5, r6, r7}
	subs			r9, r9, 1
	stmia			r12!, {r2, r3, r4, r5, r6, r7}

	bpl				.LCopy

.LEfficacy:

	vldr			d0, [r1, efficacy]	@ D0 = pLut->efficacy
	mov				r1, sp
	bl				KOLIBA_SlutEfficacy(PLT)

	@ We are done, return.

	mov				sp, fp
	pop				{r4, r5, r6, r7, r8, r9, r10, fp, pc}

	@ If (pLut->divisor != 1.0)

.LDivide:

	@ D0 still equals pLut->divisor; D1 still equals 1.0.

	vdiv.f64		d0, d1, d0		@ D0 = 1.0 / pLut->divisor

.LImpera:

	ldrb			r8, [r9, r10]	@ Get offset of next color within pLut structure.
	add				r8, r8, r1		@ Add it to the address of the pLut argument.
	vldmia			r8, {d4, d5, d6}
	vmul.f64		d4, d4, d0
	vmul.f64		d5, d5, d0
	vmul.f64		d6, d6, d0
	subs			r9, r9, 1
	vstmia			r12!, {d4, d5, d6}
	bpl				.LImpera

	b				.LEfficacy


	.align	2

.pLutOffsets:

	@ These are offsets within the pLut structure
	@ arranged in reverse sLut order (reverse, because
	@ we are moving from the top to the bottom).

	.byte	white, yellow, magenta, red, cyan, green, blue, black

	.size	KOLIBA_ConvertPlutToSlut, .-KOLIBA_ConvertPlutToSlut
