/*

	effinc.s - ARM EABI 32-bit version

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
	@		  R0    = address of output SLUT
	@		  R1    = address of (const) EFFILUT
	@
	@ On Exit:
	@
	@		 R0    = address output SLUT or NULL
	@
	@	Because of the way natural contrasts work,
	@	we apply an inversion of the EFFILUT.

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.equ	effilut,	(8*8)
	.equ	lvars,	(effilut)

	.align	2
	.global	KOLIBA_ApplyNaturalContrasts
	.type	KOLIBA_ApplyNaturalContrasts, %function

KOLIBA_ApplyNaturalContrasts:

	@ No EFFILUT, no action. KOLIBA_ApplyEfficacies will check R0 for NULL.

	cmp		r1, 0
	moveq	r0, 0
	bxeq	lr

	push	{r0, lr}
	sub		sp, sp, lvars

	@ Create an inverted EFFILUT.

	mov		r0, sp
	bl		KOLIBA_InvertEfficacies

	@ Load the addresses of things we need to pass to KOLIBA_ApplyEfficacies.

	adr		r12, .Stuff
	ldmia	r12, {r1-r3}

.HerrGot:

	add		r2, pc, r2			@ R2 = GOT
	ldr		r1, [r2, r1]		@ R1 = address of KOLIBA_NaturalContrastSlut
	ldr		r3, [r2, r3]		@ R3 = address of KOLIBA_IdentitySlut
	mov		r2, r0				@ R2 = inverted EFFILUT
	ldr		r0, [sp, lvars]		@ R0 = output SLUT

	bl		KOLIBA_ApplyEfficacies

	@ Hasta la Pasta!

	add		sp, sp, lvars+4		@ No need to pop R0.
	pop		{pc}









/*	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	subs	r3, r1, #0
	ldr	r4, .L6
	sub	sp, sp, #68
.LPIC0:
	add	r4, pc, r4
	moveq	r0, r3
	beq	.L1
	mov	r5, r0
	mov	r0, sp
	bl	KOLIBA_InvertEfficacies(PLT)
	ldr	r2, .L6+4
	ldr	r3, .L6+8
	ldr	r1, [r4, r2]
	ldr	r3, [r4, r3]
	mov	r2, r0
	mov	r0, r5
	bl	KOLIBA_ApplyEfficacies(PLT)
.L1:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, pc}
.L7:
	.align	2
.L6:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	KOLIBA_NaturalContrastSlut(GOT)
	.word	KOLIBA_IdentitySlut(GOT)
*/

.Stuff:

	.word	KOLIBA_NaturalContrastSlut(GOT)
	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)
	.word	KOLIBA_IdentitySlut(GOT)


	.size	KOLIBA_ApplyNaturalContrasts, .-KOLIBA_ApplyNaturalContrasts
