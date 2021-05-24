/*

	indwxyz.s - ARM EABI 32-bit version

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
	@		  R0   = address of output xyz
	@		  R1   = address of (const) input xyz
	@		  R2   = address of (const) array of double (base)
	@		  R3   = address of (const) 24-bit flag vector array (in 32-bit ints)
	@		 [SP]  = address of dim[3]
	@		[SP+4] = address of (const) KOLIBA_FLINDEX
	@		[SP+8] = address of (const) unsigned short findex
	@
	@ On Exit:
	@
	@		  R0  = address of output xyz or NULL
	@
	@	Find the index, apply it to fLuts, set flag,
	@	and call ApplyXyz.
	@
	@	Do not check for NULL pointers because we
	@	may be called millions of times, so it is
	@	up to the caller to make sure everything is
	@	set up correctly.

	.arch 	armv6
	.text
	.align	2
	.global	KOLIBA_IndexedShortXyz
	.syntax unified
	.arm
	.fpu 	vfp
	.equ	lvars, (3*8*9+4)
	.type	KOLIBA_IndexedShortXyz, %function

KOLIBA_IndexedShortXyz:

	push	{r0, r2, r3, r4, lr}
	.equ	oldsp, (5*4)			@ bytes pushed

	@ Find the index

	ldr		r4, [sp,oldsp+8]		@ R4 = findex[]
	sub		sp, sp, lvars			@ SP = temp XYZ, SP+3*8 = FLUT, aligned
	mov		r3, 0
	ldr		r2, [sp,lvars+oldsp]	@ R2 = address of dim[3]
	mov		r0, sp					@ R0 = address of temp XYZ, R1 = address of input XYZ
	bl		KOLIBA_GetFindex(PLT)

	@  R0 = index

	lsl		r12, r0, 1
	ldr		r1, [sp,lvars+oldsp+4]	@ R1 = FLINDEX
	add		r0, r0, r0, lsl 1		@ R0 = index*3
	ldr		r2, [sp,lvars+4]		@ R2 = base
	add		r1, r1, r0, lsl 5		@ R1 = address of flindex + index*24*4
	ldrh	r4, [r4,r12]			@ R4 = findex[index], HWORD extended to WORD
	add		r0, sp, 3*8				@ R0 = address of temp FLUT
	bl		KOLIBA_ConvertFlindexToFlut(PLT)

	@ R0 = address of temp FLUT

	ldr		r3, [sp,lvars+8]		@ R3 = flag vector array
	mov		r2, r0					@ R2 = temp FLUT
	ldr		r3, [r3, r4, lsl 2]		@ R3 = flag vector value from flags[findex[index]]
	mov		r1, sp					@ R1 = temp XYZ
	ldr		r0, [sp,lvars]			@ R0 = output
	bl		KOLIBA_ApplyXyz(PLT)

	@ Hasta la Pasta!

	add		sp, sp, lvars+12		@ Clear off local vars, plus R0, R2, R3.
	pop		{r4, pc}

	.size	KOLIBA_IndexedShortXyz, .-KOLIBA_IndexedShortXyz
