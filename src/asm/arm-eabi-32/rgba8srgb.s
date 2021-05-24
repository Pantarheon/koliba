/*

	rgba8srgb.s - ARM EABI 32-bit version

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
	@		R0 = address of output RGBA 8bpp pixel(s)
	@		R1 = address of (const) input RGBA 8bpp pixel(s)
	@		R2 = number of pixels
	@
	@	Convert linear RGBA8 to sRGB RGBA8.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Rgba8ToSrgb
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Rgba8ToSrgb, %function

KOLIBA_Rgba8ToSrgb:

	@ If either pointer is NULL, return NULL.
	@ Else if the count is 0, return output pointer.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	cmpne			r2, 0
	bxeq			lr

	@ Else, do the job!

	push			{r0, r4, r5, r6, r7, lr}

	mov				r4, r0
	mov				r5, r1
	mov				r6, r2

.LFromOuter:

	mov				r7, 3

.LFromInner:

	bl				.LDiv
	bl				KOLIBA_LinearToSrgb
	bl				.LMul
	subs			r7, r7, 1
	bne				.LFromInner

	ldrb			r12, [r5], 1
	subs			r6, r6, 1
	strb			r12, [r4], 1
	bne				.LFromOuter

	pop				{r0, r4, r5, r6, r7, pc}

	.size	KOLIBA_Rgba8ToSrgb, .-KOLIBA_Rgba8ToSrgb


	.arch armv6
	.text
	.align	2
	.syntax unified
	.arm
	.fpu vfp
	.type	.LDiv, %function

.LDiv:

	ldrb			r12, [r5], 1
	vmov			s0, r12
	vldr.64			d1, .LDiv255
	vcvt.f64.u32	d0, s0
	vmul.f64		d0, d1
	bx				lr

.LDiv255:

	.word	0x10101010, 0x3F701010	@ = 1 / 255 = 0.00392157

	.size	.LDiv, .-.LDiv




	.arch armv6
	.text
	.align	2
	.syntax unified
	.arm
	.fpu vfp
	.type	.LMul, %function

.LMul:

	adr				r12, .LLittle
	vldmia.f64		r12, {d4-d7}
	vcmp.f64		d0, d4
	vmrs			APSR_nzcv, fpscr
	movle			r12, 0
	ble				.LMulled
	vcmp.f64		d0, d5
	vmrs			APSR_nzcv, fpscr
	movge			r12, 255
	vmlalt.f64		d6, d0, d7
	vcvtlt.u32.f64	s0, d6
	vmovlt			r12, s0

.LMulled:

	strb			r12, [r4], 1
	bx				lr


	.align	2

.LLittle:

	.word	0x10101010, 0x3F601010	@ < 0.5f / 255.0f = 0.00196078...

.LOne:

	@ Actually, this is slightly less than 1.0,
	@ but the nearest integer of it multiplied
	@ by 255.0 is 255.

	.word	0xEFEFEFF0, 0x3FEFEFEF	@ = 254.5f / 255.0f = 0.998039

.LHalf:

	.word	0x00000000, 0x3FE00000	@ = 0.5f

.L255:

	.word	0x00000000, 0x406FE000	@ = 255

	.size	.LMul, .-.LMul

	@ On Input:
	@
	@		R0 = address of output RGBA 8bpp pixel(s)
	@		R1 = address of (const) input RGBA 8bpp pixel(s)
	@		R2 = number of pixels
	@
	@	Convert sRGB RGBA8 to linear RGBA8.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_SrgbToRgba8
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_SrgbToRgba8, %function

KOLIBA_SrgbToRgba8:

	@ If either pointer is NULL, return NULL.
	@ Else if the count is 0, return output pointer.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	cmpne			r2, 0
	bxeq			lr

	@ Else, do the job!

	push			{r0, r4, r5, r6, r7, lr}

	mov				r4, r0
	mov				r5, r1
	mov				r6, r2

.LToOuter:

	mov				r7, 3

.LToInner:

	bl				.LDiv
	bl				KOLIBA_SrgbToLinear
	bl				.LMul
	subs			r7, r7, 1
	bne				.LToInner

	ldrb			r12, [r5], 1
	subs			r6, r6, 1
	strb			r12, [r4], 1
	bne				.LToOuter

	pop				{r0, r4, r5, r6, r7, pc}

	.size	KOLIBA_SrgbToRgba8, .-KOLIBA_SrgbToRgba8

