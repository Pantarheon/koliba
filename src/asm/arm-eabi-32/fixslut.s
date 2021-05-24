/*

	fixslut.s - ARM EABI 32-bit version

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
	@		R0 = address of sLut or fLut
	@
	@ On Exit:
	@
	@		R0 = same
	@
	@	Strictly speaking, we can use this code for fixing
	@	any sequence of 24 doubles, which both, sLut and fLut,
	@	are.
	@
	@	We are simply checking if the absolute value of any
	@	of the twenty-four is smaller than or equal to 2.0^-32,
	@	i.e. 1.0/4294967296.0 (with ^ as a power operator), and
	@	if so, we replace it with a zero. Otherwise we leave it
	@	as is.
	@
	@	This is useful because, contrary to popular opinion,
	@	floating point math is only precise with numbers that
	@	are either powers of 2 or, if we are lucky, certain sums
	@	of such numbers. In what we are doing with color grading,
	@	numbers as small as those fixed here are most likely
	@	errors, which only slow us down, so it is usually a good
	@	idea to replace them with zeros.

	.arch	armv6
	.text
	.align	2
	.global	KOLIBA_FixFlut
	.global	KOLIBA_FixSlut
	.syntax unified
	.arm
	.fpu 	vfp
	.type	KOLIBA_FixFlut, %function
	.type	KOLIBA_FixSlut, %function

KOLIBA_FixFlut:
KOLIBA_FixSlut:

	@ Check for a NULL pointer.

	cmp			r0, 0
	bxeq		lr

	@ Do the job.

	mov			r12, 24
	vldr.64		d1, .Little
	mov			r2, 0
	mov			r3, 0

.Loop:

	subs		r12, r12, 1
	addne		r1, r0, r12, lsl 3
	vldrne.64	d0, [r1]
	bxeq		lr
	vabs.f64	d0, d0
	vcmp.f64	d0, d1
	vmrs		APSR_nzcv, FPSCR
	strdle		r2, [r1]
	b			.Loop

	.align		3

.Little:

	.word		0, 0x3df00000

	.size		KOLIBA_FixSlut, .-KOLIBA_FixSlut
	.size		KOLIBA_FixFlut, .-KOLIBA_FixFlut

