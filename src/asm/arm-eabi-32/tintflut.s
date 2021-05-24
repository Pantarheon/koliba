/*

	tintflut.s - ARM EABI 32-bit version

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
	@		 R0  = address of output fLut
	@		 R1  = address of (const) input tint (xyz/cmy)
	@		 R2  = boolean (char) invert
	@		 R3  = boolean (char) flut
	@		 D0  = saturation
	@		 D1  = tinge
	@		 D2  = light
	@		[SP] = address of RGB rec, or NULL
	@
	@ On Exit:
	@
	@		 R0  = address of output fLut or NULL
	@
	@	Convert a tint, tinge, light, saturation
	@	to a FLUT, either directly or via SLUT.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_TintToFlut
	.syntax unified
	.arm
	.fpu vfp
	@ The size of a FLUT is 3*8*8.
	@ The size of VERTICES is 4*8.
	@ That adds up to (3*8)*8 + (4)*8 = (3*8+4)*8.
	.equ	locvars,	((3*8+4)*8)
	.type	KOLIBA_TintToFlut, %function

KOLIBA_TintToFlut:

	@ Return error if no fLut

	cmp				r0, 0
@	mov				r12, sp
	bxeq			lr

	push			{r4-r10, lr}
	mov				r5, r1					@ R5  = input tint
	ldr				r1, [sp,8*4]			@ R1  = address of rec, or NULL
	sub				sp, sp, locvars			@ SP  = temp fLut and vertices
	mov				r4, r0					@ R4  = output fLut
	add				r10, sp, 3*8*8			@ R10 = temp vertices
	mov				r9, sp					@ R9  = temp fLut
	vpush.f64		{d1-d2}
	ldr				r12, .LGot

	and				r6, r2, 0xFF			@ R6  = invert

.LHerrGot:

	add				r12, pc, r12
	vldr.64			d1, .LOne

	ands			r7, r3, 0xFF			@ R7  = flut, flags = flut
	vsub.f64		d0, d1, d0				@ D0  = 1.0 - saturation
	ldreq			r8, .LSE				@ R8  = KOLIBA_SlutEfficacy
	ldrne			r8, .LFE				@ R8  = KOLIBA_FlutEfficacy
	ldreq			r2, .LRS				@ R2  = KOLIBA_ConvertRecToSlut
	ldrne			r2, .LRF				@ R2  = KOLIBA_ConvertRecToFlut
	vpush.f64		{d0}
	mov				r0, r9
	ldr				r2, [r12, r2]
	ldr				r8, [r12, r8]
	blx				r2						@ Convert Rec to sLut or fLut

	mov				r1, r0
	vpop.f64		{d0}					@ 1.0 - saturation
	blx				r8						@ Apply saturation of sLut or fLut

	@ If not going straight to fLut, point vertices at temp sLut.

	cmp				r7, 0
	moveq			r0, r10					@ temp vertices
	moveq			r1, r9					@ temp sLut/fLut
	bleq			KOLIBA_SlutToVertices(PLT)

	@ If tint is NULL, copy temp sLut/fLut to output and return.

	cmp				r5, 0
	mov				r0, r4					@ This sets R0 for the rest of this code.
	beq				.LDiverge

	vldr.64			d3, [sp]				@ D3  = tinge
	vldmia.f64		r5, {d0-d2}				@ D0-D2 = tint
	vnmul.f64		d0, d3, d0				@ D0  = -tint.r*tinge, etc.
	vnmul.f64		d1, d3, d1
	vnmul.f64		d2, d3, d2
	vstmia.f64		r9, {d0-d2}				@ temp fLut.black

	@ If we are NOT inverting, subtract tint channels
	@ from 1.0. Either way, multiply them by light.
	@ Though treat it somewhat differently if bypassing
	@ the sLut and using the fLut differently.
	cmp				r7, 0					@ flut?
	vldr.64			d7, .LOne
	vsubne.f64		d0, d0, d0
	vmoveq.f64		d0, d7
	cmp				r6, 0					@ invert ?
	vmov.f64		d1, d0
	vldr.64			d3, [sp, 8]				@ D3 = light
	vldmia.f64		r5, {d4-d6}				@ {D4-D6} = tint
	vmov.f64		d2, d0					@ {D0-D2} = 1.0
	vsubeq.f64		d4, d7, d4
	add				r12, r9, 3*8*7			@ R12 = temp fLut.black
	vsubeq.f64		d5, d7, d5
	vmla.f64		d0, d3, d4
	vsubeq.f64		d6, d7, d6
	vmla.f64		d1, d3, d5
	vmla.f64		d2, d3, d6
	vstmia.f64		r12, {d0-d2}

	@ Now diverge based on whether going straight to fLut,
	@ with not going straight being the likely default.
	@
	@ R0 is still pointing at the output, from line 125,
	@ and continues to do so even after SlutToFlut.
	@
	@ On the other hand, we cannot rely on line 139 for
	@ the state of the processor flags because we may have
	@ branched to here from line 126, skipping over line
	@ 139, so we need to test the value of R7 here.

.LDiverge:

	cmp				r7, 0
	bne				.LDirectfLut

	mov				r1, r10					@ vertices
	bl				KOLIBA_ConvertSlutToFlut(PLT)
	add				sp, sp, locvars+2*8
	pop				{r4-r10, pc}

.LDirectfLut:

	vldmia.f64		r9!, {d0-d7}
	vstmia.f64		r4!, {d0-d7}
	vldmia.f64		r9!, {d0-d7}
	vstmia.f64		r4!, {d0-d7}
	vldmia.f64		r9, {d0-d7}
	vstmia.f64		r4, {d0-d7}
	add				sp, sp, locvars+2*8
	pop				{r4-r10, pc}


	.align	3

.LOne:

	.word	0, 0x3ff00000


.LGot:

	.word	_GLOBAL_OFFSET_TABLE_-(.LHerrGot+8)

.LSE:

	.word	KOLIBA_SlutEfficacy(GOT)

.LFE:

	.word	KOLIBA_FlutEfficacy(GOT)

.LRS:

	.word	KOLIBA_ConvertGrayToSlut(GOT)

.LRF:

	.word	KOLIBA_ConvertGrayToFlut(GOT)

	.size	KOLIBA_TintToFlut, .-KOLIBA_TintToFlut
