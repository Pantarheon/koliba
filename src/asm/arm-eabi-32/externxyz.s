/*

	externxyz.s - ARM EABI 32-bit version

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
	@		 R0		= address of output XYZ
	@		 R1		= address of (const) input XYZ
	@		 R2		= address of (const) KOLIBA_FFLUT
	@		 R3		= n
	@		[SP]	= m
	@		[SP,4]	= external function to call (if NULL, must not be called)
	@		[SP,8]	= address of (const) parameters to pass to the function (may be NULL)
	@
	@ After we push some registers and create a local XYZ, we will have:

	.equ	pushedregs, (6*4)
	.equ	localvars, (3*8)
	.equ	stackbytes, (pushedregs+localvars)


	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ExternalXyz
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ExternalXyz, %function

KOLIBA_ExternalXyz:

	@ If no input or no output, cannot do anything, so just throw up hands.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	ldrne			r12, [sp, 4]
	bxeq			lr

	@ Else, if no external function, just branch to PolyXyz with n+m fLuts.
	@ It will then return directly to whomever called us.

	cmp				r12, 0
	ldreq			r12, [sp]
	addeq			r3, r12, r3
	beq				KOLIBA_PolyXyz(PLT)

	@ Else, do the job. Start by calling PolyXyz
	@ with the first n FFLUTs.

	push			{r4, r5, r6, r7, r8, lr}
	sub				sp, sp, localvars
	mov				r4, r0
	mov				r0, sp
	mov				r5, r1
	mov				r6, r2
	mov				r7, r3
	mov				r8, r12
	bl				KOLIBA_PolyXyz(PLT)

	@ Call the external function, passing to it SP in R0
	@ and its own parameters.

	mov				r0, sp			@ This is redundant, but better safe than sorry
	ldr				r1, [sp,stackbytes+8]
	blx				r8

	@ Call PolyXyz with the remaining m FFLUTS.

	ldr				r3, [sp,stackbytes]			@ m
	add				r2, r6, r7, lsl 3			@ address of FFLUT[n]
	mov				r1, sp						@ our temp XYZ
	mov				r0, r4						@ output XYZ

	@ Because we still have a local stack variable (temp XYZ),
	@ we cannot just jump to PolyXyz, we have to call it, then
	@ clean up the stack and restore saved registers.

	bl				KOLIBA_PolyXyz(PLT)

	@ We do not have to restore R0 because PolyXyz has returned it
	@ (unless there was an error, but if so, we need to return the
	@ NULL it has returned).

	add				sp, sp, localvars

	@ And, hasta la pasta!

	pop				{r4, r5, r6, r7, r8, pc}

	.size	KOLIBA_ExternalXyz, .-KOLIBA_ExternalXyz
