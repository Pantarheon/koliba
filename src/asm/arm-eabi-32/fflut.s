/*

	fflut.s - ARM EABI 32-bit version

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
	@		R0 = address of (const) KOLIBA_FLUT
	@
	@ On Exit:
	@
	@		R0 = flags

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_FlutFlags
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_FlutFlags, %function

KOLIBA_FlutFlags:

	cmp			r0, 0
	addne		r1, r0, 3*8*8
	mvneq		r0, 0xFF000000
	bxeq		lr

	mov			r12, 3*7
	mov			r3, 8			@ = 1 << 3 because of the next line.
	mov			r0, 7			@ These bits should always be set.

.Loop:

	subs		r12, 1
	vldmdbpl	r1!, {d0}
	bxmi		lr

	vcmp.f64	d0, #0
	vmrs		APSR_nzcv, fpscr
	lslne		r2, r3, r12
	orrne		r0, r2
	b			.Loop

	.size	KOLIBA_FlutFlags, .-KOLIBA_FlutFlags
