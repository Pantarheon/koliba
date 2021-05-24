/*

	ldxapply.s - ARM EABI 32-bit version

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
	@		 R0  = address of output XYZ
	@		 R1  = address of (const) foreground XYZ
	@		 R2  = address of (const) background XYZ
	@		 R3  = address of lumidux to apply
	@		[SP] = address of Rec, or NULL
	@
	@ While Processing:
	@
	@		 R4  = ldx->ldirection (signed)
	@		 R5  = ldx->sdirection (signed)

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ApplyLumidux
	.syntax unified
	.arm
	.fpu	vfp
	.type	KOLIBA_ApplyLumidux, %function

KOLIBA_ApplyLumidux:

	@ Save some registers.

	push			{r4-r6, lr}
	.equ			oldsp, 4*4		@ Size of pushed registers

	ldrsb			r4, [r3]		@ R4 = LDIR (lumidux ternary flag)
	ldrsb			r5, [r3, 4]		@ R5 = SDIR (lumidux ternary flag)

	@ Decide what to do based on ldx flags.

	cmp				r4, 0
	blt				.LNoLumi

	@ We need to calculate luminosity.

	bl				.LCalcLuminosity

	@ See if we also need to calculate saturance.

	cmp				r5, 0
	blt				.LMask

	@ Yes, we do, so save the result of luminosity.
	@ Then calculate the saturance, and find the
	@ geometric average of the two.

	vmov.f64		d1, d0
	bl				.LCalcSaturance
	vmul.f64		d1, d0, d1
	vsqrt.f64		d0, d1

.LMask:

	ldrb			r4, [r3, 3]		@ R4 = ldx binary Mask
	cmp				r4, 0

	vmovne.f64		d1, d0
	moveq			r3, 3			@ R3 = Number of doubles in XYZ
	vmovne.f64		d2, d0
	vstmiane.f64	r0, {d0-d2}
	bleq			KOLIBA_Interpolate(PLT)

	@ Hasta la Pasta!

	pop				{r4-r6, pc}

.LNoLumi:

	cmp				r5, 0
	adrpl			lr, .LMask		@ This assembles to <subpl lr, pc, .+8-.LMask>

	@ The "return" address has been set in the previous line
	@ to .LMask, so do *NOT* change this bpl to blpl!!!
	bpl			.LCalcSaturance

	@ Nothing to do with Lumidux, so
	@ just copy the input to the output.

	vldmia.f64		r1, {d0-d2}
	vstmia.f64		r0, {d0-d2}

	@ Hasta la Pasta!

	pop				{r4-r6, pc}


.LCalcSaturance:

	@ N.B. Do not change D1, and R0-R5 unless
	@      you want to mess up the caller!

	@ Find out whether to base this on the
	@ foreground or the background.

	ldrb			r12, [r3, 5]	@ R12 = ldx->sbase
	cmp				r12, 0
	vldmiaeq.f64	r2, {d3-d5}		@ Use background
	vldmiane.f64	r1, {d3-d5}		@ Use foreground

	@ Find the maximum and the minimum channel.

	vcmp.f64		d3, d4
	vmrs			APSR_nzcv, FPSCR

	vmovge.f64		d0, d3
	vmovle.f64		d2, d3
	vmovlt.f64		d0, d4
	vmovgt.f64		d2, d4

	vcmp.f64		d0, d5
	vmrs			APSR_nzcv, FPSCR
	vcmp.f64		d2, d5
	vmovlt.f64		d0, d5
	vmrs			APSR_nzcv, FPSCR
	ldrb			r12, [r3, 7]
	vmovgt.f64		d2, d5

	@ Calculate chrominance, and if R12 is zero and
	@ maximum is non-zero, calculate the saturation
	@ by dividing chrominance by the maximum.
	@
	@ Chrominance = max - min.

	cmp				r12, 0
	vmoveq.f64		d5, d0
	vsub.f64		d0, d0, d2
	add				r12, r3, 24		@ R12 = address of ldx->slow
	bne				.LNoSat

	vcmp.f64		d5, #0
	vmrs			APSR_nzcv, FPSCR
	vdivne.f64		d0, d0, d5

.LNoSat:

	@ Load lo and hi from ldx slow and shigh.
	@ Also, load ldx->sspeed to R12 and ldx->sdirection
	@ to R6 in preparation  of adjusting for the speed
	@ by branching to the right part of .LSpeed.

	vldmia			r12, {d2-d3}
	ldrb			r12, [r3, 6]	@ R12 = sspeed
	mov				r6, r5			@ R6  = sdirection

.LTheRest:

	@ If lo > hi, swap lo and hi.

	vcmp.f64		d2, d3
	vmrs			APSR_nzcv, FPSCR

	vmovgt.f64		d4, d2
	vmovgt.f64		d2, d3
	vmovgt.f64		d3, d4

	@ If saturance (D0) is less than or equal to lo (D2),
	@ let saturance = 0 and branch to .LSpeed to check
	@ whether it needs to be replaced with 1.0.

	vcmp.f64		d0, d2
	vmrs			APSR_nzcv, FPSCR

	vsuble.f64		d0, d0, d0
	ble				.LSpeed

	@ Else if saturance (D0) is greater than or equal to
	@ lo (D3), replace it with 1.0 and branch to .LSpeed.

	vcmp.f64		d0, d3
	vmrs			APSR_nzcv, FPSCR

	vldrge.f64		d0, .LOne
	bge				.LSpeed

	@ Else, scale the saturance (D0) to (sat-lo)/(hi-lo),
	@ i.e. (D0-D2)/(D3-D2).

	vsub.f64		d0, d0, d2
	vsub.f64		d2, d3, d2
	vdiv.f64		d0, d0, d2

	@ Continue to .LSpeed.

.LSpeed:

	@ If the direction (R6) is greater than 0,
	@ subtract D0 from 1.

	cmp				r6, 0
	vldrgt.f64		d2, .LOne
	vsubgt.f64		d0, d2, d0

	@ R12 tells us which speed to apply. Valid
	@ values are [0-6]. If we have an invalid
	@ value, we just return.

	cmp				r12, 6
	addls			pc, pc, r12, lsl 2
	bx				lr

	vmul.f64		d0, d0
	vmul.f64		d0, d0
	vmul.f64		d0, d0
	bx				lr

	b				.LSpeed4
	b				.LSpeed5

	vsqrt.f64		d0, d0

.LSpeed5:

	vsqrt.f64		d0, d0

.LSpeed4:

	vsqrt.f64		d0, d0
	bx				lr

	.align			3

.LOne:

	.double			1.0

.LCalcLuminosity:

	@ N.B. Do not change R0-R5 unless
	@      you want to mess up the caller!

	@ Find out whether to base this on the
	@ foreground or the background.

	ldrb			r12, [r3, 1]	@ R12 = ldx->lbase
	cmp				r12, 0
	vldmiaeq.f64	r2, {d3-d5}		@ Use background
	vldmiane.f64	r1, {d3-d5}		@ Use foreground

	@ Convert these to luminosity. BTW, we do not
	@ check if the luminosity factors add up to 1,
	@ we expect the caller to do it because it
	@ makes more sense to do it once outside the
	@ loop that calls us.

	ldr				r6, [sp, oldsp]	@ R6 = Rec or NULL
	cmp				r6, 0
	ldreq			r12, .LGot
	ldreq			r6, .LRec2020

.LHerrGot:

	addeq			r12, pc, r12
	ldreq			r6, [r12,r6]	@ R6 = Rec, default or otherwise

	@ Load lo and hi from ldx llow and lhigh.
	@ Also, load ldx->lspeed to R12 and ldx->ldirection
	@ to R6 in preparation  of adjusting for the speed
	@ by branching to the right part of .LSpeed.

	vldmia.f64		r6, {d0-d2}
	vmul.f64		d0, d3, d0
	add				r12, r3, 8		@ address of ldx->llow
	vmla.f64		d0, d4, d1
	mov				r6, r4			@ R6  = ldirection
	vmla.f64		d0, d5, d2

	vldmia			r12, {d2-d3}
	ldrb			r12, [r3, 2]	@ R12 = lspeed

	@ The rest is the same for here as for saturance,
	@ so just jump there.

	b				.LTheRest

	.align	2

.LGot:

	.word	_GLOBAL_OFFSET_TABLE_-(.LHerrGot+8)

.LRec2020:

	.word	KOLIBA_Rec2020(GOT)

	.size	KOLIBA_ApplyLumidux, .-KOLIBA_ApplyLumidux
