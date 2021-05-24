/*

	mkcube.s - ARM EABI 32-bit version

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
	@		 R0		= address of output KOLIBA_RGB array
	@		 R1		= m
	@		 R2		= n
	@		 R3		= o
	@		[SP]	= address of (KOLIBA_MAKEVERTEX)()
	@		[SP+4]	= address of parameters to (KOLIBA_MAKEVERTEX)()
	@
	@ On Exit:
	@
	@		 R0		address of output, or NULL.
	@
	@@@@@@@@
	@ N.B: @
	@@@@@@@@
	@
	@	The address pointed at by R0 must have enough available
	@	memory for (m+1)(n+1)(o+1) KOLIBA_RGBs. If it does not,
	@	a crash is likely! That memory has to be allocated, whether
	@	dynamically or in some other way, by the caller of the
	@	function.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_MakeCube
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_MakeCube, %function

KOLIBA_MakeCube:

	@ Check for NULL pointers (except params) and 0 values.

	cmp				r3, 0
	cmpne			r2, 0
	ldrne			r12, [sp]			@ function pointer
	cmpne			r1, 0
	cmpne			r12, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Also check for values above 256.

	cmp				r1, 256
	cmpls			r2, 256
	cmpls			r3, 256
	movls			r12, sp
	movhi			r0, 0
	bxhi			lr

	@ Omnia conserva conservanda.

	push			{r0, r4, r5, r6, r7, r8, r9, r10, r11, lr}
	vpush			{d8-d10}

	ldmia			r12, {r5-r6}	@ R5  = function pointer, R6 = function parameters
	mov				r12, 0x3f800000
	mov				r4, r0			@ R4  = address of next KOLIBA_RGB
	vmov			s16, r1
	vmov			s18, r2
	vmov			s20, r3
	vmov.f32		s1, r12
	mov				r7, r1			@ R7  = m (red)
	mov				r8, r2			@ R8  = n (green)
	mov				r9, r3			@ R9  = o (blue)
	vcvt.f64.u32	d8, s16
	vcvt.f64.f32	d0, s1			@ D0  = 1.0
	vcvt.f64.u32	d9, s18
	vcvt.f64.u32	d10, s20
	mov				r10, 0			@ R10 = k (red)
	mov				r11, 0			@ R11 = j (green)
	vdiv.f64		d8, d0, d8		@ D8  = 1.0 / (double)m (red)
	vdiv.f64		d9, d0, d9		@ D9  = 1.0 / (double)n (green)
	vdiv.f64		d10, d0, d10	@ D10 = 1.0 / (double)o (blue)

	push			{r10-r11}		@ [SP+24] = i (blue), plus alignment
	push			{r10-r11}		@ [SP+16] = rgb.b
	push			{r10, r11}		@ [SP+8]  = rgb.g
	push			{r10-r11}		@ [SP]    = rgb.r

	@ We have three logical loops. They all come back here.
	@ So, let us just use three labels for clarity of purpose.

.LoopI:

	@ The outermost loop (i loop in the C source code) controls rgb.b.

.LoopJ:

	@ The middle loop (j loop in C) controls rgb.g.

.LoopK:

	@ The innermost loop (k loop in C) controls rgb.r.
	@
	@ And remember, we run the K loop m+1 times,
	@ the J loop n+1 times, and the I loop o+1 times.
	@
	@ We also increase the reds more than the greens,
	@ and the greens more than the blues. That is what
	@ the .cube file format specification tells us to
	@ do, so we do it to be compatible.

	mov				r0, r4			@ Current KOLIBA_RGB to be written to by (KOLIBA_MAKEVERTEX)()
	mov				r1, sp
	mov				r2, r6
	add				r4, r4, 24		@ Next KOLIBA_RGB, if any.
	blx				r5				@ Call (KOLIBA_MAKEVERTEX)()

	cmp				r10, r7			@ k == m ?
	vpop.f64		{d0}
	moveq			r10, 0			@ if yes, k = 0
	vaddne.f64		d0, d8, d0
	addne			r10, r10, 1		@ else, k++
	vpushne.f64		{d0}
	bne				.LoopK			@ No.

	vpop.f64		{d1}			@ Yes.
	cmp				r11, r8			@ j == n ?

	@ Start new K loop with K = 0.

	vsub.f64		d0, d0, d0
	vaddne.f64		d1, d9, d1
	addne			r11, r11, 1
	vpushne.f64		{d0-d1}
	bne				.LoopJ			@ No.

	vpop.f64		{d2}			@ Yes.
	pop				{r12}

	@ Start new J loop with J = 0, and since it is also
	@ starting a new K loop, we have already set J = 0
	@ and can just copy it from K=D0 to J=D1.

	vmov.f64		d1, d0			@ = 0.0
	cmp				r12, r9			@ i == o ?
	mov				r11, 0			@ j = 0
	addne			r12, r12, 1
	vaddne.f64		d2, d10, d2
	pushne			{r12}
	vpushne			{d0-d2}
	bne				.LoopI			@ No.

	@ Once we have exhausted the I loop, we are finished,
	# so no new J or K loop is necessary.

	add				sp, sp, 4		@ Yes, all done.

	@ Restitue conservata.

	vpop			{d8-d10}
	pop				{r0, r4, r5, r6, r7, r8, r9, r10, r11, pc}

	.size	KOLIBA_MakeCube, .-KOLIBA_MakeCube
