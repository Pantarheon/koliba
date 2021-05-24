/*

	mofarslt.s - ARM EABI 32-bit version

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
	@		  R1    = address of (const) GRAY or NULL
	@		  R2    = unsigned byte flags
	@		  D0    = primary saturation
	@		  D1	= secondary saturation
	@
	@ On Exit:
	@
	@		 R0    = address output SLUT or NULL

	.arch	armv6
	.syntax unified
	.arm
	.fpu vfp

	.text

	.equ	SLUT,	(8*3*8)

	.align	2
	.global	KOLIBA_MonoFarbaToSlut
	.type	KOLIBA_MonoFarbaToSlut, %function

KOLIBA_MonoFarbaToSlut:

	@ Output SLUT may not be NULL.

	cmp			r0, 0
	bxeq		lr

	@ Save preservanda. Create local SLUT.

	push		{r5, r6, r7, lr}
	mov			r6, r2				@ R6 = flags
	mov			r7, r0				@ R7 = output
	sub			sp, sp, SLUT
	vpush		{d0-d1}

	@ Produce a gray-scale SLUT. R1 is already set.

	add			r0, sp, 2*8
	bl			KOLIBA_ConvertGrayToSlut

	@ Output primary vertices. R0 is local SLUT.
	@ You still need to find IdentitySlut.

	ldr			r12, .Got
	vpop		{d0}
	ldr			r5, .IdSlut

.HerrGot:

	add			r12, pc, r12
	mov			r2, r0
	mov			r3, r6
	ldr			r5, [r12, r5]
	mov			r0, r7
	mov			r1, r5
	bl			KOLIBA_InterpolateSlutVertices

	@ Output secondary vertices. R0 is already set.

	vpop		{d0}
	mvn			r3, r6
	mov			r1, r5
	mov			r2, sp
	and			r3, r3, 0xFF
	bl			KOLIBA_InterpolateSlutVertices

	@ Hasta la Pasta!

	add			sp, sp, SLUT
	pop			{r5, r6, r7, pc}


	.align	2

.Got:

	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IdSlut:

	.word	KOLIBA_IdentitySlut(GOT)
	.size	KOLIBA_MonoFarbaToSlut, .-KOLIBA_MonoFarbaToSlut
