/*

	gamma.s - ARM EABI 32-bit version

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

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	2
	.global	KOLIBA_PrepareGammaParameters
	.type	KOLIBA_PrepareGammaParameters, %function

	@ On Input:
	@
	@		R0 = address of output parameters (p)
	@		R1 = address of (const) input parameters (q)
	@
	@	We scale the parameters, so any q < 0 becomes p = 1.0 - q,
	@	and all others become p = 1.0 / (1.0 + q).

KOLIBA_PrepareGammaParameters:

	@ Reject NULL parameters.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Do the conversion.

	vldmia.f64		r1, {d0-d2}			@ D0 = q.x, D1 = q.y, D2 = q.z

	vcmp.f64		d0, #0
	vldr.64			d3, .LOne			@ D3 = 1.0
	vmrs			APSR_nzcv, FPSCR
	vaddpl.f64		d0, d3, d0			@ if q.x >= 0.0, D0  = 1.0 + q.x
	vsubmi.f64		d0, d3, d0			@ if q.x <  0.0, p.x = 1.9 - q.x
	vdivpl.f64		d0, d3, d0			@ if q.x >= 0.0, p.x = 1.0 / D0 = 1.0 / (1.0 + q.x)

	vcmp.f64		d1, #0
	vmrs			APSR_nzcv, FPSCR
	vaddpl.f64		d1, d3, d1			@ if q.y >= 0.0, D1  = 1.0 + q.y
	vsubmi.f64		d1, d3, d1			@ if q.y <  0.0, p.y = 1.9 - q.y
	vdivpl.f64		d1, d3, d1			@ if q.y >= 0.0, p.y = 1.0 / D1 = 1.0 / (1.0 + q.y)

	vcmp.f64		d2, #0
	vmrs			APSR_nzcv, FPSCR
	vaddpl.f64		d2, d3, d2			@ if q.z >= 0.0, D2  = 1.0 + q.z
	vsubmi.f64		d2, d3, d2			@ if q.z <  0.0, p.z = 1.9 - q.z
	vdivpl.f64		d2, d3, d2			@ if q.z >= 0.0, p.z = 1.0 / D2 = 1.0 / (1.0 + q.z)

	vstmia.f64		r0, {d0-d2}
	bx				lr

	.align	3

.LOne:

	.word	0, 0x3ff00000

	.size	KOLIBA_PrepareGammaParameters, .-KOLIBA_PrepareGammaParameters



	.align	2
	.global	KOLIBA_Gamma
	.type	KOLIBA_Gamma, %function

	@ On Input:
	@
	@		R0 = address of input/output XYZ
	@		R1 = address of (const) powers
	@
	@	We raise the input to powers and write it back to output.
	@	Because this may happen during a live show, we change
	@	nothing if there is a NULL pointer. While this will not
	@	produce the desired effect, at least it will not disrupt
	@	the show.
	@
	@	After all, the show must go on.
	@
	@	Additionally, since ARM chips do not have a hardware-based
	@	power function, the CLib pow is slow here. And since raising
	@	any value to the power of 1 does not change that value, we
	@	test if that is what we are asked to do. And if so, we skip
	@	over such a request.

KOLIBA_Gamma:

	cmp				r0, 0
	cmpne			r1, 0
	vldrne.64		d4, .LOne
	bxeq			lr

	push			{r0, r4, r5, lr}
	vpush.f64		{d8-d13}

	vldmia.f64		r0, {d8-d10}
	mov				r4, r0
	vldmia.f64		r1, {d11-d13}

	vcmp.f64		d11, d4
	vmrs			APSR_nzcv, FPSCR
	beq				.Ly

	vmov.f64		d0, d8
	vmov.f64		d1, d11
	bl				pow(PLT)
	vmov.f64		d8, d0

.Ly:

	vcmp.f64		d12, d4
	vmrs			APSR_nzcv, FPSCR
	beq				.Lz

	vmov.f64		d1, d12
	vmov.f64		d0, d9
	bl				pow(PLT)
	vmov.f64		d9, d0

.Lz:

	vcmp.f64		d13, d4
	vmrs			APSR_nzcv, FPSCR
	beq				.LDone

	vmov.f64		d1, d13
	vmov.f64		d0, d10
	bl				pow(PLT)
	vmov.f64		d10, d0

.LDone:

	vstmia.f64		r4, {d8-d10}
	vpop.f64		{d8-d13}
	pop				{r0, r4, r5, pc}

	.size	KOLIBA_Gamma, .-KOLIBA_Gamma
