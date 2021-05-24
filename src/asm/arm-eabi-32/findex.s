/*

	findex.s - ARM EABI 32-bit version

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
	@		R0 = address of output XYZ
	@		R1 = address of (const) input XYZ
	@		R2 = address of (const) dimension array
	@		R3 = address of (optional) output index array
	@
	@ On Exit:
	@
	@		R0 = index

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_GetFindex
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_GetFindex, %function

KOLIBA_GetFindex:

	@ If either XYZ is NULL, we cannot do anything, so just return -3.

	cmp				r1, 0

	@ If everything is valid, we need to remember where our output is.
	@ So let us save it to R12. We do it unconditionally in the (fairly
	@ reasonable) hope that everything turns out valid. Presumably,
	@ the processor can do this while it is figuring out the state of the
	@ flags, which might speed us up in the expected case of valid input.

	mov				r12, r0

	@ Again, expecting things to likely be valid, we load the input values to
	@ double registers, but we do it only after we have verified its address
	@ is not NULL (which we have done in the first line of code). That
	@ accomplished, we may also reuse R1 for other purposes than holding
	@ the input address.

	vldmiane.f64	r1, {d0-d2}

	cmpne			r0, 0

	@ Yet another action we can take unconditionally based on the assumption
	@ everything is likely to be valid.

	vldr.f64		d6, .LOne			@ D6 = 1.0

	mvneq			r0, 2				@ R0 = ~2 = -3
	bxeq			lr

	@ We need a valid dimension array. So, first check if we have one. If not, copy the input
	@ to the output and return -2. If, however, we do have one, we load it to registers (after
	@ saving them on the stack first) in preparation of what comes next.

	cmp				r2, 0
	pushne			{r4, r5, r6, lr}
	ldmne			r2, {r4-r6}
	mvneq			r0, 1				@ R0 = ~1 = -2
	vstmiaeq.f64	r12, {d0-d2}
	vmovne			s6, r4
	bxeq			lr

	@ We have a dimension array. But we still need to assure all dimensions are non-zero.
	@ If not, we set the return value to -1 and jump toward the end of the procedure to copy
	@ the dimensions to the indices (if they are present). Such indices can be rejected as
	@ invalid by other procedures, which can prevent additional errors. And of course we will
	@ also copy the unchanged input to the output. This will also free R2 for other use.
	@
	@ While doing that, we can start converting the dimensions to doubles. That will be a waste
	@ of time if any of the dimensions is invalid, but we prioritize the speed of error-free
	@ code over the speed of returning an error.

	cmp				r4, 0
	vmov			s8, r5
	vcvt.f64.u32	d3, s6				@ D3 = R4 = dim[0]
	cmpne			r5, 0
	mov				r1, r5
	vmov			s10, r6
	cmpne			r6, 0
	vcvt.f64.u32	d4, s8				@ D4 = R5 = R1 = dim[1]
	mov				r2, r6
	vcvt.f64.u32	d5, s10				@ D5 = R6 = R2 = dim[2]
	mvneq			r0, 0				@ R0 = ~0 = -1
	beq				.Lfinish

	@ Everything checks out, so multiply x-z by dim[0-2].

	vmul.f64		d0, d0, d3			@ D0 = x * dim[0]
	vmul.f64		d1, d1, d4			@ D1 = y * dim[1]
	vmul.f64		d2, d2, d5			@ D2 = z * dim[2]

	@ If x*dim[0] < 1.0, i = 0, x = x*dim[0], and we skip to figuring out j.

	vcmp.f64		d0, d6
	vmrs			APSR_nzcv, FPSCR
	movmi			r4, 0				@ R4 = i = 0, D0 = x
	vsubpl.f64		d3, d3, d6			@ D3 = (double)dim[0] - 1.0

	@ Else if x*dim[0] >= dim[0] - 1, i = dim[0] - 1, else i = (int)x*dim[0].
	@ In either case, x = x*dim[0] - (double)i.

	vcmppl.f64		d0, d3		

	bmi				.Lj

	vmrs			APSR_nzcv, FPSCR
	vcvtmi.u32.f64	s14, d0
	subpl			r4, r4, 1
	vcvtmi.f64.u32	d3, s14
	vmovmi			r4, s14
	vsub.f64		d0, d3				@ R4 = i, D0 = x

.Lj:

	@ If y*dim[1] < 1.0, j = 0, y = y*dim[1], and we skip to figuring out k.

	vcmp.f64		d1, d6
	vmrs			APSR_nzcv, FPSCR
	vsubpl.f64		d4, d4, d6			@ D4 = (double)dim[1] - 1.0
	movmi			r5, 0				@ R5 = j = 0, D1 = y

	@ Else if y*dim[1] >= dim[1] - 1, j = dim[1] - 1, else j = (int)y*dim[1].
	@ In either case, y = y*dim[1] - (double)j.

	vcmppl.f64		d1, d4		

	bmi				.Lk

	vmrs			APSR_nzcv, FPSCR
	vcvtmi.u32.f64	s14, d1
	subpl			r5, r5, 1
	vcvtmi.f64.u32	d4, s14
	vmovmi			r5, s14
	vsub.f64		d1, d4				@ R5 = j, D1 = y

.Lk:

	@ If z*dim[2] < 1.0, z = 0, z = z*dim[2], and we skip to figuring out the index.

	vcmp.f64		d2, d6

	@ At this point we can calculate the first part of the index,
	@ so by the time we get to .Lindex, we do not have to wait
	@ for the pipeline to finish this first part.
	@
	@ Of course, we can only do it here because no prior part of this code
	@ has branched to .Lindex. And because nothing between this next line
	@ and .Lindex changes R1.

	mla				r1, r4, r1, r5		@ R1 = i*dim[1]+j

	vmrs			APSR_nzcv, FPSCR
	vsubpl.f64		d5, d5, d6			@ D5 = (double)dim[2] - 1.0
	movmi			r6, 0				@ R6 = k = 0, D2 = z

	@ Else if z*dim[2] >= dim[2] - 1, k = dim[2] - 1, else k = (int)z*dim[2].
	@ In either case, z = z*dim[2] - (double)k.

	vcmppl.f64		d2, d5		

	bmi				.Lindex

	vmrs			APSR_nzcv, FPSCR
	vcvtmi.u32.f64	s14, d2
	subpl			r6, r6, 1
	vcvtmi.f64.u32	d5, s14
	vmovmi			r6, s14
	vsub.f64		d2, d5				@ R5 = j, D1 = y

.Lindex:

	@ Finish calculating the index (R1 = i*dim[1]+j already).

	mla				r0, r1, r2, r6		@ R0 = (i*dim[1]+j)*dim[2]+k

.Lfinish:

	@ If ind is not NULL, copy the i, j, k (if everything checks out)
	@ or the dim m, n, o (if everything does not check out) to it.
	@
	@ Also, copy x, y, z (modified or original) to the output.

	cmp				r3, 0
	vstmia.f64		r12, {d0-d2}
	stmiane			r3, {r4-r6}

	@ Hasta la pasta!

	pop				{r4, r5, r6, pc}

	.align	3

.LOne:

	.word	0, 0x3FF00000

	.size	KOLIBA_GetFindex, .-KOLIBA_GetFindex
