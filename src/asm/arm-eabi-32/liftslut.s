/*

	liftslut.s - ARM EABI 32-bit version

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
	@		  R0    = address of input/output SLUT
	@		  R1    = flags (byte)
	@		  D0    = lift
	@
	@ On Exit:
	@
	@		 R0    = unchanged

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_ApplyLiftToSlutVertices
	.type	KOLIBA_ApplyLiftToSlutVertices, %function

KOLIBA_ApplyLiftToSlutVertices:

	@ No SLUT, no service. Otherwise, if lift = 0,
	@ do not waste time.

	cmp			r0, 0
	vcmpne.f64	d0, #0
	vldr.f64	d7, .One
	vmrsne		APSR_nzcv, FPSCR
	bxeq		lr

	@ Prepare local variables

	mov			r2, r0		@ R2 = SLUT
	mov			r3, 8		@ R3 = counter
	vsub.f64	d7, d0		@ D7 = 1.0 - lift

.TheLoop:

	lsrs		r1, r1, 1	@ is a flag set?
	bcc			.Next		@ no, continue.

	@ Flag is set, apply lift.

	vldmia.f64	r2, {d1-d3}
	vmov.f64	d4, d0
	vmov.f64	d5, d0
	vmov.f64	d6, d0

	vmla.f64	d4, d7, d1
	vmla.f64	d5, d7, d2
	vmla.f64	d6, d7, d3

	vstmia.f64	r2, {d4-d6}

.Next:

	subs		r3, r3, 1
	add			r2, r2, 3*8
	bne			.TheLoop

	@ Hasta la Pasta!

	bx			lr


	.align	3

.One:

	.double	1.0

	.size	KOLIBA_ApplyLiftToSlutVertices, .-KOLIBA_ApplyLiftToSlutVertices
