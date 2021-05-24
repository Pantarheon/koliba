/*

	linsrgb.s - ARM EABI 32-bit version

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
	@		D0 = a double to convert
	@
	@ On Output:
	@
	@		D0 = converted double
	@

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_LinearToSrgb
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_LinearToSrgb, %function

KOLIBA_LinearToSrgb:

	@ Compare the double to 0.0031308

	adr				r0, .Div24
	vldmia.64		r0, {d1-d3}

	/*
		D1 = 1.0 / 2.4
		D2 = 0.0031308
		D3 = 12.92
	*/

	vcmp.f64		d0, d2

	@ If less, return D0 * 12.92.
	@ Else, raise D0 ^ 1.0 / 2.4, ...

	pushge			{r4, lr}
	vmullt.f64		d0, d3, d0
	adrge			r4, .Shift
	bxlt			lr

	bl				pow(PLT)

	@ ... multiply by 1.055, subtract 0.055, and return the result.

	vldmia.64		r4, {d1-d2}

	/*
		D1 = 0.055
		D2 = 1.055
	*/

	vnmls.f64		d1, d0, d2
	vmov.f64		d0, d1

	@ Hasta la pasta!

	pop				{r4, pc}

	.align 3

.Div24:

	.double	0.416666666666666667	@ 1.0 / 2.4

.DblComp1:

	.double	0.0031308

.TinyConv:

	.double	12.92

.Shift:

	.double	0.055

.Scale:

	.double	1.055

.DblComp2:

	.double	0.04045

.Power:

	.double	2.4

	.size	KOLIBA_LinearToSrgb, .-KOLIBA_LinearToSrgb


	.align	2
	.global	KOLIBA_SrgbToLinear
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_SrgbToLinear, %function

KOLIBA_SrgbToLinear:

	@ Compare to 0.04045.

	adr				r0, .TinyConv
	vldmia.64		r0, {d1-d5}

	/*
		D1 = 12.92
		D2 = 0.055
		D3 = 1.055
		D4 = 0.04045
		D5 = 2.4
	*/

	vcmp.f64		d0, d4

	@ If less or equal, divide by 12.92 and return.
	@ Else, add 0.055, divide by 1.055, raise to the
	@ 2.4 power and return (actually, we can just
	@ jump to pow and let it return).

	vmovgt.f64		d1, d3
	vaddgt.f64		d0, d0, d2
	vdiv.f64		d0, d0, d1
	vmovgt.f64		d1, d5

	@ Hasta la Pasta!

	bxle			lr
	b				pow(PLT)


	.size	KOLIBA_SrgbToLinear, .-KOLIBA_SrgbToLinear
