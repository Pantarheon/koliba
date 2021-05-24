/*

	frange.s - ARM EABI 32-bit version

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
	@		R0 = address of KOLIBA_VERTICES
	@		R1 = address of (const) starting color (KOLIBA_RGB)
	@		R2 = address of (const) ending color (KOLIBA_RGB)
	@
	@ On Exit:
	@
	@		R0 = unchanged
	@
	@	We only return NULL if fLut is NULL. If starting color
	@	is NULL, we just use black. If ending color is NULL, we
	@	use white.

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_FarbaRange
	.type	KOLIBA_FarbaRange, %function
KOLIBA_FarbaRange:

	cmp				r0, 0
	bxeq			lr

	cmp				r1, 0
	vsubeq.f64		d0, d0
	vldmiane.f64	r1, {d0-d2}
	vmoveq.f64		d1, d0
	vmoveq.f64		d2, d0

	cmp				r2, 0
	mov				r1, r0			@ R1 = address of vertices
	vldreq.f64		d3, .One
	vldmiane.f64	r2, {d3-d5}
	ldr				r2, [r1,4]!		@ pre-indexed load of vertices.blue
	vmoveq.f64		d4, d3
	vmoveq.f64		d5, d3

	@ Blue

	@ Alternate between R2 and R3 to, hopefully, run faster.
	ldr				r3, [r1,4]!
	vstmia.f64		r2!, {d0-d1}
	vstmia.f64		r2, {d5}

	@ Green

	ldr				r2, [r1,4]!
	vstmia.f64		r3!, {d0}
	vstmia.f64		r3!, {d4}
	vstmia.f64		r3, {d2}

	@ Cyan

	ldr				r3, [r1,4]!
	vstmia.f64		r2!, {d0}
	vstmia.f64		r2, {d4-d5}

	@ Red

	ldr				r2, [r1,4]!
	vstmia.f64		r3!, {d3}
	vstmia.f64		r3, {d1-d2}

	@ Magenta

	ldr				r3, [r1,4]
	vstmia.f64		r2!, {d3}
	vstmia.f64		r2!, {d1}
	vstmia.f64		r2, {d5}

	@ Yellow

	vstmia.f64		r3!, {d3-d4}
	vstmia.f64		r3, {d2}

	@ Hasta la Pasta!

	bx				lr

	.align	3

.One:

	.double	1.0

	.size	KOLIBA_FarbaRange, .-KOLIBA_FarbaRange
