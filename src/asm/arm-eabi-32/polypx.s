/*

	polypx.s - ARM EABI 32-bit version

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
	@		  R0   = address of output pixel
	@		  R1   = address of (const) input pixel
	@		  R2   = address of (const) KOLIBA_FFLUT
	@		  R3   = n
	@		 [SP]  = PixelToXyz transform function or NULL
	@		[Sp,4] = XyzToPixel transform function or NULL
	@
	@ On Output:
	@
	@		  R0   = address output xyz or NULL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_PolyPixel
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_PolyPixel, %function

KOLIBA_PolyPixel:

	@ Reject NULL pointers

	cmp				r1, 0
	moveq			r0, 0
	movne			r12, sp
	cmpne			r0, 0
	pushne			{r4, r5, r6, lr}
	ldmiane			r12, {r5-r6}	@ R5 = PixelToXyz, R6 = XyzToPixel
	bxeq			lr

	@ Convert pixel to XYZ, call KOLIBA_PolyXyz
	@ and convert the result back to pixel.
	@
	@ But if the caller passed transform functions,
	@ use those for the corresponding conversions.

	cmp				r5, 0
	mov				r4, r0

	vldmiaeq.f64	r1, {s6-s8}
	subne			sp, sp, 3*8
	vcvteq.f64.f32	d0, s6
	vcvteq.f64.f32	d1, s7
	vcvteq.f64.f32	d2, s8
	vpusheq.f64		{d0, d1, d2}

	mov				r0, sp
	push			{r2, r3}	@ This is unconditional because (r5)() may change the flags.
	blxne			r5
	pop				{r2, r3}
	cmp				r0, sp		@ Did the transform (if any) return the non-error value?
	mov				r1, sp
	movne			r0, 0
	addne			sp, sp, 3*8
	popne			{r4, r5, r6, pc}

	bl				KOLIBA_PolyXyz(PLT)

	cmp				r0, 0		@ If it returned NULL, return the NULL as well.
	addeq			sp, sp, 3*8
	popeq			{r4, r5, r6, pc}

	cmp				r6, 0
	mov				r0, r4
	vldmiaeq.f64	sp, {d0, d1, d2}

	vcvteq.f32.f64	s6, d0
	movne			r1, sp
	vcvteq.f32.f64	s7, d1
	vcvteq.f32.f64	s8, d2

	vstmiaeq.f32	r4, {s6-s8}
	blxne			r6

	cmp				r0, r4		@ Did the transform (if any) return the non-error value?
	add				sp, sp, 3*8
	movne			r0, 0
	pop				{r4, r5, r6, pc}

	.size	KOLIBA_PolyPixel, .-KOLIBA_PolyPixel
