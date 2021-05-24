/*

	mkidcube.s - ARM EABI 32-bit version

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
	@		 R0		= address of output KOLIBA_RGB array
	@		 R1		= m
	@		 R2		= n
	@		 R3		= o

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_MakeIdentityCube
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_MakeIdentityCube, %function

KOLIBA_MakeIdentityCube:

	@ Check for valid inputs. But also start prepping your vars.

	cmp				r0, 0

	vmov			s0, r1
	vmov			s2, r2
	vmov			s4, r3

	bxeq			lr

	sub				r1, r1, 1
	sub				r2, r2, 1
	sub				r3, r3, 1

	cmp				r1, 255
	vcvt.f64.u32	d0, s0			@ D0  = m
	cmpls			r2, 255
	vcvt.f64.u32	d1, s2			@ D1  = n
	cmpls			r3, 255
	vcvt.f64.u32	d2, s4			@ D2  = o
	movhi			r0, 0
	bxhi			lr

	push			{r0, r4, r5, lr}

	mov				r4, 0			@ R4  = i = 0
	mov				r5, 0			@ R5  = j = 0
	mov				r12, 0			@ R12 = k = 0

	vmov			d3, r4, r5		@ D3  = x = i / m = 0.0
	vmov			d4, r4, r5		@ D4  = y = j / n = 0.0
	vmov			d5, r4, r5		@ D5  = z = k / o = 0.0

.LoopI:

.LoopJ:

.LoopK:

	cmp				r12, r3
	vstmia.f64		r0!, {d3-d5}
	addls			r12, r12, 1
	movhi			r12, 0
	vmov			s13, r12
	vcvt.f64.u32	d6, s13
	vdiv.f64		d5, d6, d2		@ z = k / o
	bls				.LoopK

	cmp				r5, r2
	addls			r5, r5, 1
	movhi			r5, 0
	vmov			s13, r5
	vcvt.f64.u32	d6, s13
	vdiv.f64		d4, d6, d1		@ y = j / n
	bls				.LoopJ

	cmp				r4, r1
	addls			r4, r4, 1
	vmovls			s13, r4
	vcvtls.f64.u32	d6, s13
	vdivls.f64		d3, d6, d0		@ x = i / m
	bls				.LoopI

	pop				{r0, r4, r5, pc}

	.size	KOLIBA_MakeIdentityCube, .-KOLIBA_MakeIdentityCube
