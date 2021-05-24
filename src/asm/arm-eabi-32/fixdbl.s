@	Copyright 2019 G. Adam Stanislav
@	All rights reserved
@
@	Redistribution and use in source and binary forms,
@	with or without modification, are permitted provided
@	that the following conditions are met:
@
@	1. Redistributions of source code must retain the
@	above copyright notice, this list of conditions
@	and the following disclaimer.
@
@	2. Redistributions in binary form must reproduce the
@	above copyright notice, this list of conditions and
@	the following disclaimer in the documentation and/or
@	other materials provided with the distribution.
@
@	3. Neither the name of the copyright holder nor the
@	names of its contributors may be used to endorse or
@	promote products derived from this software without
@	specific prior written permission.
@
@	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
@	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
@	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
@	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
@	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
@	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
@	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
@	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
@	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
@	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
@	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
@	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_FixDoubles
	.global KOLIBA_NetDoubles
	.syntax unified
	.arm
	.type	KOLIBA_FixDoubles, %function
	.type	KOLIBA_NetDoubles, %function

	@ This is written for the Raspbian OS (32-bit),
	@ but can probably work with many other systems
	@ using ARM-EABI-32.
	@
	@ ARM processors can run in an LSB-first or an MSB-first
	@ mode. This code assumes LSB-first. However, you can
	@ define BIGENDIAN to assembly for the MSB-first use.

KOLIBA_FixDoubles:
KOLIBA_NetDoubles:

.ifdef	BIGENDIAN

	cmp		r1, 0
	moveq	r0, 0
	bx		lr

.else


	push	{r4, r5, lr}
	cmp		r1, 0
	moveq	r0, 0
	cmpne	r0, 0
	movne	ip, r0
	popeq	{r4, r5, pc}

.Loop:

	ldrd	r2, r3, [ip]
	rev		r5, r2
	rev		r4, r3
	subs	r1, r1, 1
	strd	r4, r5, [ip], 8
	bne	.Loop

	pop	{r4, r5, pc}

.endif

	.size	KOLIBA_NetDoubles, .-KOLIBA_NetDoubles
	.size	KOLIBA_FixDoubles, .-KOLIBA_FixDoubles
