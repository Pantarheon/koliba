/*

	appxyz.s - ARM EABI 32-bit version

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
	@		R0 = address of output xyz
	@		R1 = address of (const) input xyz
	@		R2 = address of (const) KOLIBA_FLUT
	@		R3 = flags
	@
	@ On Output:
	@
	@		R0 = address output xyz or NULL
	@
	@	We do the math for the factors for which the
	@	corresponding flag bit is zero. We, however,
	@	use the first factor vector regardless of its
	@	flag.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ApplyXyz
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ApplyXyz, %function

KOLIBA_ApplyXyz:

	@ Check for NULL pointers

	cmp			r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ If the FLUT is 0, just copy the input to the output.
	@ Not much else you can do.

	cmp			r2, 0
	vldmiaeq	r1, {d0-d2}
	beq			.Lend

	@ Initialize the output to V[000]. This will occasionally
	@ be all we need to do, so test for that possibility.
	@ Then again, the flags may disagree, and demand
	@ we replace at least some of these with zeroes.
	@ That should only be demanded for some special effects,
	@ not just because V[000] = {0.0, 0.0, 0.0}. After all,
	@ if it does there is no need to be replacing zeroes with
	@ other zeroes. Therefore, normally the black portion
	@ of the flags should be set even when V[000] contains
	@ any zeroes.

	vldmia		r2!, {d0-d2}
	lsrs		r3, r3, 1
	vsubcc.f64	d0, d0, d0
	lsrs		r3, r3, 1
	vsubcc.f64	d1, d1, d1
	lsrs		r3, r3, 1
	vsubcc.f64	d2, d2, d2

	@ So, if this really is all, the flags are already zero
	@ and the last shift turned on the processor ZF.

	beq			.Lend

	@ Get x, y, z

	vldmia		r1, {d3-d5}		@ D3 = x, D4 = y, D5 = z

	@ Save the conservanda

	vpush		{d8-d9}

	@ Now, do the math

	tst			r3, 7			@ Red
	vldmiane	r2, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d3, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d3, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d3, d9

	tst			r3, 7			@ Green
	addne		r1, r2, 24
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d4, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d4, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d4, d9

	tst			r3, 7			@ Blue
	addne		r1, r2, 48
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d5, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d5, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d5, d9

	@ Again, this will often be all we need to do,
	@ and if so, we already have the zero flag set.

	beq			.Ldone

	@ Calculate xy, but only if needed. In this test,
	@ remember R3 has been right-shifted twelve times.

	tst			r3, 0x007		@ Used with yellow...
	tsteq		r3, 0xE00		@ ...but also with white (there multiplied by z)
	vmulne.f64	d6, d3, d4		@ D6 = xy

	tst			r3, 7			@ Yellow
	addne		r1, r2, 72
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d6, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d6, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d6, d9
	beq			.Ldone

	tst			r3, 7			@ Magenta
	addne		r1, r2, 96
	vmulne.f64	d3, d3, d5		@ D3 = xz
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d3, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d3, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d3, d9
	beq			.Ldone

	tst			r3, 7			@ Cyan
	addne		r1, r2, 120
	vmulne.f64	d4, d4, d5		@ D4 = yz
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d4, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d4, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d4, d9
	beq			.Ldone

	tst			r3, 7			@ White
	addne		r1, r2, 144
	vmulne.f64	d5, d5, d6		@ D5 = xyz
	vldmiane	r1, {d7-d9}
	lsrs		r3, r3, 1
	vmlacs.f64	d0, d5, d7
	lsrs		r3, r3, 1
	vmlacs.f64	d1, d5, d8
	lsrs		r3, r3, 1
	vmlacs.f64	d2, d5, d9

	@ Conservata restoranda

.Ldone:

	vpop		{d8-d9}

	@ Return the results

.Lend:

	vstmia		r0, {d0-d2}

	@ Hasta la pasta!

	bx			lr


	.size	KOLIBA_ApplyXyz, .-KOLIBA_ApplyXyz
