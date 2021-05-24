/*

	cbreset.s - ARM EABI 32-bit version

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
	@		R0 = address of channel blend
	@		R1 = address of (const) input modificand matrix, or NULL
	@		R2 = address of (const) input modifier gains vertex, or NULL

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	3

.GetIdentityChannelBlend:

	ldr			r3, .Got
	ldr			r2, .IdBlend

.HerrGot:

	add			r3, pc, r3
	ldr			r2, [r3, r2]

	bx			lr

	.align	2

.Got:

	.word	_GLOBAL_OFFSET_TABLE_-(.HerrGot+8)

.IdBlend:

	.word	KOLIBA_IdentityChannelBlend(GOT)

	.align	3
	.global	KOLIBA_ResetChannelBlend
	.type	KOLIBA_ResetChannelBlend, %function

KOLIBA_ResetChannelBlend:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr

	bl			.GetIdentityChannelBlend

	mov			r3, r0

	vldmia.f64	r2!, {d0-d7}
	vstmia.f64	r3!, {d0-d7}
	vldmia.f64	r2,  {d0-d4}	@ Includes efficacy
	mov			r2,  0			@ flags
	vstmia.f64	r3!, {d0-d4}
	str			r2,  [r3]

	bx			r1

	.size	KOLIBA_ResetChannelBlend, .-KOLIBA_ResetChannelBlend

	.align	3
	.global	KOLIBA_ResetChannelBlendRed
	.type	KOLIBA_ResetChannelBlendRed, %function

KOLIBA_ResetChannelBlendRed:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr

	bl			.GetIdentityChannelBlend

	vldmia.f64	r2, {d0-d3}
	ldr			r2, [r0,(3*4+1)*8]	@ flags
	vstmia.f64	r0, {d0-d3}
	uxtb		r3, r2, ror 24		@ Copy the "all" flag...
	mov			r2, r3, lsl 8		@ ...to green flag...
	orr			r2, r2, r3, lsl 16	@ ...and to blue flag...
	str			r2, [r0,(3*4+1)*8]	@ ...but the red and all flags are 0.

	bx			r1

	.size	KOLIBA_ResetChannelBlendRed, .-KOLIBA_ResetChannelBlendRed

	.align	3
	.global	KOLIBA_ResetChannelBlendGreen
	.type	KOLIBA_ResetChannelBlendGreen, %function

KOLIBA_ResetChannelBlendGreen:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr

	bl			.GetIdentityChannelBlend

	add			r2, r2, 4*8			@ .green
	add			r3, r0, 4*8
	vldmia.f64	r2, {d0-d3}
	ldr			r2, [r0,(3*4+1)*8]	@ flags
	vstmia.f64	r3, {d0-d3}
	uxtb		r2, r2, ror 24		@ Move all flag to red flag...
	orr			r2, r2, r2, lsl 16	@ ...and copy to blue flag.
	str			r2, [r0,(3*4+1)*8]

	bx			r1

	.size	KOLIBA_ResetChannelBlendGreen, .-KOLIBA_ResetChannelBlendGreen

	.align	3
	.global	KOLIBA_ResetChannelBlendBlue
	.type	KOLIBA_ResetChannelBlendBlue, %function

KOLIBA_ResetChannelBlendBlue:

	cmp			r0, 0
	mov			r1, lr
	bxeq		lr

	bl			.GetIdentityChannelBlend

	add			r2, r2, 2*4*8		@ .blue
	add			r3, r0, 2*4*8
	vldmia.f64	r2, {d0-d3}
	ldr			r2, [r0,(3*4+1)*8]	@ flags
	vstmia.f64	r3, {d0-d3}
	uxtb		r2, r2, ror 24		@ Move all flag to red flag...
	orr			r2, r2, r2, lsl 8	@ ...and copy to green flag.
	str			r2, [r0,(3*4+1)*8]

	bx			r1

	.size	KOLIBA_ResetChannelBlendBlue, .-KOLIBA_ResetChannelBlendBlue
