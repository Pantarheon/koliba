/*

	partslt.s - ARM EABI 32-bit version

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
	@		  R0   = address of output SLUT
	@		  R1   = address of (const) input SLUT
	@		  R2   = flags (byte)

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_SlutSelect
	.type	KOLIBA_SlutSelect, %function

KOLIBA_SlutSelect:

	@ If any of the inpute is 0, just return R0.

	cmp				r0, 0
	cmpne			r1, 0
	andsne			r2, 0xFF
	bxeq			lr

	@ Copy those of the eight vertices of the input
	@ into the output that have a flag set. That
	@ means we return when the loop ends or when
	@ no flags remain, whichever comes first.

	mov				r12, 8		@ The counter
	mov				r3, r0		@ So we do not lose the output in R0.

.Loop:

	lsrs			r2, r2, 1
	vldmiacs.f64	r1, {d0-d2}
	vstmiacs.f64	r3, {d0-d2}

	subsne			r12, r12, 1
	bxeq			lr
	add				r1, r1, 3*8
	add				r3, r3, 3*8
	b				.Loop

	.size	KOLIBA_SlutSelect, .-KOLIBA_SlutSelect
