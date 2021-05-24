/*

	unindxyz.s - ARM EABI 32-bit version

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
	@		 R0  = address of output xyz
	@		 R1  = address of (const) input xyz
	@		 R2  = address of (const) array of fLuts
	@		 R3  = address of (const) array of flags
	@		[SP] = address of dim[3]
	@
	@ On Exit:
	@
	@		R0 = address of output xyz or NULL
	@
	@	Find the index, apply it to fLuts and flags
	@	and call ApplyXyz.

	.arch 	armv6
	.eabi_attribute 28, 1
	.text
	.align	2
	.global	KOLIBA_NonindexedXyz
	.syntax unified
	.arm
	.fpu 	vfp
	.equ	lvar, (3*8)
	.type	KOLIBA_NonindexedXyz, %function

KOLIBA_NonindexedXyz:

	push	{r0, r2, r3, lr}
	.equ	oldsp, 4*4

	sub		sp, sp, lvar		@ SP = address of temp XYZ
	mov		r3, 0
	ldr		r2, [sp,lvar+oldsp]	@ R2 = dim[3]
	mov		r0, sp				@ R0 = temp XYZ, R1 = input XYZ
	bl		KOLIBA_GetFindex(PLT)

	@ R0 = index

	ldr		r3, [sp,lvar+8]		@ R3  = array of flag vectors
	add		r12, r0, r0, lsl 1	@ R12 = R0 * 3
	ldr		r2, [sp,lvar+4]		@ R2  = address of fLuts, 3*8*8 bytes each
	ldr		r3, [r3, r0, lsl 2]	@ R3  = flags
	add		r2, r2, r12, lsl 6	@ R2  = fLuts[index*3*8*8]
	mov		r1, sp				@ R1  = temp XYZ
	ldr		r0, [sp,lvar]		@ R0  = output XYZ
	bl		KOLIBA_ApplyXyz(PLT)

	@ Hasta la Pasta!

	add		sp, sp, lvar+12		@ No need to pop R0, R2, R3.
	pop		{pc}

	.size	KOLIBA_NonindexedXyz, .-KOLIBA_NonindexedXyz
