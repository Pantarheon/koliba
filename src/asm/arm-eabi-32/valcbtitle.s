/*

	valcbtitle.s - ARM EABI 32-bit version

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
	@		  R0    = address of (const) title string
	@
	@ On Exit:
	@
	@		 R0    = 0 if title valid, 1 if invalid, -1 if too long or too short.
	@
	@	A text string is a valid .cube title if and only if it is
	@	less than 242 characters long and contains no double quote,
	@	no line feed and no carriage return.

	.arch armv6
	.file	"valcbtitle.c"
	.text
	.align	2
	.global	KOLIBA_CheckCubeTitle
	.arch armv6
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_CheckCubeTitle, %function

KOLIBA_CheckCubeTitle:

	cmp		r0, 0
	addne	r1, r0, 1
	addne	r2, r0, 242
	ldrbne	r0, [r0]
	cmpne	r0, 0
	beq		.WrongLength

.Loop:

	cmp		r0, 0
	bxeq	lr

	cmp		r0, 10		@ Line feed?
	cmpne	r0, 13		@ Carriage return?
	cmpne	r0, 34		@ Double quote?
	moveq	r0, 1
	bxeq	lr

	cmp		r1, r2
	ldrbne	r0, [r1], 1
	bne		.Loop

.WrongLength:

	mvn		r0, 0
	bx		lr

	.size	KOLIBA_CheckCubeTitle, .-KOLIBA_CheckCubeTitle
