/*

	unindwxyz.s - ARM EABI 32-bit version

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
	@		  R2   = address of (const) array of fLut
	@		  R3   = address of (const) array of flag vectors
	@		 [SP]  = address of dim[3]
	@		[SP+4] = ignored
	@		[SP+8] = address of (const) array of flag vector indices
	@
	@ On Exit:
	@
	@		  R0  = address of output xyz or NULL
	@
	@	Find the index, apply it to fLuts and flags,
	@	and call ApplyXyz.
	@
	@	Do not check for NULL pointers because we
	@	may be called millions of times, so it is
	@	up to the caller to make sure everything is
	@	set up correctly.

	.arch 	armv6
	.text
	.align	2
	.global	KOLIBA_NonindexedShortXyz
	.syntax unified
	.arm
	.fpu 	vfp
	.equ	lvars, (3*8)
	.type	KOLIBA_NonindexedShortXyz, %function

KOLIBA_NonindexedShortXyz:

	push	{r0, r2, r3, lr}
	.equ	oldsp, 4*4				@ number of bytes pushed

	@ Find the index

	sub		sp, sp, lvars			@ SP = temp XYZ
	mov		r3, 0
	ldr		r2, [sp,lvars+oldsp]	@ R2 = address of dim[3]
	mov		r0, sp					@ R0 = address of temp XYZ, R1 = address of input XYZ
	bl		KOLIBA_GetFindex(PLT)

	ldr		r3, [sp,lvars+oldsp+8]	@ R3 = flag vector index array
	lsl		r1, r0, 1
	ldr		r2, [sp,lvars+4]		@ R2 = fLut[]
	add		r0, r0, r0, lsl 1		@ R0 = index*3
	ldr		r12, [sp,lvars+8]		@ IP = array of flag vectors
	ldrh	r3, [r3, r1]			@ R3 = address of flags[index]
	mov		r1, sp					@ R1 = address of temp XYZ
	add		r2, r2, r0, lsl 6		@ R2 = address of fLut + index*lvars*8
	ldr		r3, [r12, r3, lsl 2]	@ R3 = flag vector
	ldr		r0, [sp,lvars]			@ R0 = output
	bl		KOLIBA_ApplyXyz(PLT)

	@ Hasta la Pasta!

	add		sp, sp, lvars+12
	pop		{pc}

	.size	KOLIBA_NonindexedShortXyz, .-KOLIBA_NonindexedShortXyz
