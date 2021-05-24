/*

	purery.s - ARM EABI 32-bit version

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
	@		R0 = address of output FLUT
	@		R1 = address of (const) input red VERTEX
	@
	@	The two structures may overlap.

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_ApplyPureErythropy
	.type	KOLIBA_ApplyPureErythropy, %function

KOLIBA_ApplyPureErythropy:

	@ Reject NULL pointers.

	cmp			r1, 0
	mov			r2, 0
	moveq		r0, 0
	cmpne		r0, 0
	mov			r12, r0
	mov			r3, 0
	bxeq		lr

	@ Load red vertex to {d5-d7}.

	vldmia.f64	r1, {d5-d7}
	mov			r1, 0
	vmov.f64	d4, d7
	vmov.f64	d3, d6

	@ Store 0 to black.

	stmia		r12!, {r1-r3}
	stmia		r12!, {r1-r3}

	@ Store red.

	vstmia.f64	r12!, {d5-d7}

	@ Calculate the sum, g+b, then load 1, then finish r+g+b.

	vadd.f64	d7, d6, d7
	vldr.f64	d2, .One		@ D2 = 1.0
	vadd.f64	d7, d5, d7		@ D7 = r+g+b

	@ Store green.

	vstmia.f64	r12!, {d4-d6}

	@ Calculate 1 - (r+g+b), then r+g+b - 1.
	@ Spread 1-(r+g+b) over several registers. Calculate 2 * (r+g+b - 1).
	@ And store blue.

	vsub.f64	d0, d2, d7		@ D0 = 1 - (r+g+b)
	vsub.f64	d6, d7, d2		@ D6 = r+g+b - 1
	vstmia.f64	r12!, {d3-d5}
	vmov.f64	d1, d0
	vadd.f64	d6, d6, d6		@ D6 = 2 * (r+g+b - 1)
	vmov.f64	d2, d0
	vmov.f64	d3, d0
	vmov.f64	d4, d0
	vmov.f64	d7, d6
	vmov.f64	d5, d6

	@ Store, yellow, magenta, cyan, then white.

	vstmia.f64	r12!, {d0-d4}
	vstmia.f64	r12!, {d0-d3}
	vstmia.f64	r12,  {d5-d7}

	@ Hasta la Pasta!

	bx			lr


	.align	3

.One:

	.double	1.0

	.size	KOLIBA_ApplyPureErythropy, .-KOLIBA_ApplyPureErythropy
