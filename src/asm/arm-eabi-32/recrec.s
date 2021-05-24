/*

	recrec.s - ARM EABI 32-bit version

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
	@		  R0    = address of output MATRIX
	@		  R1    = address of (const) from RGB model
	@		  R2    = address of (const) to RGB model
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX

	.arch 	armv6
	.text
	.align	2
	.global	KOLIBA_RecRec
	.syntax unified
	.arm
	.equ	matsize,	(3*4*8)
	.equ	lvars,	(2*matsize+4)
	.fpu	 vfp
	.type	KOLIBA_RecRec, %function

KOLIBA_RecRec:

	push	{r0, r4, lr}
	sub		sp, sp, lvars

	mov		r4, r2
	add		r0, sp, matsize
	bl		KOLIBA_RgbToYcc
	
	mov		r1, r4
	mov		r4, r0
	mov		r0, sp
	bl		KOLIBA_YccToRgb

	mov		r2, r0
	mov		r1, r4
	ldr		r0, [sp, lvars]
	bl		KOLIBA_MultiplyMatrices

	@ Hasta la Pasta!

	add		sp, sp, lvars+4
	pop		{r4, pc}

	.size	KOLIBA_RecRec, .-KOLIBA_RecRec
