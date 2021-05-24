/*

	xyz32bgra.s - ARM EABI 32-bit version

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
	@		R0 = address of output BGR(A)32PIXEL
	@		R1 = address of (const) input XYZ
	@		R2 = address of double conversion function, or NULL
	@
	@ On Output:
	@
	@		R0 = output BGR(A)32PIXEL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_XyzToBgra32Pixel
	.global	KOLIBA_XyzToAbgr32Pixel
	.arch 	armv6
	.syntax unified
	.arm
	.fpu 	vfp
	.type	KOLIBA_XyzToBgra32Pixel, %function
	.type	KOLIBA_XyzToAbgr32Pixel, %function

KOLIBA_XyzToAbgr32Pixel:

	@ The only difference between ABGR and BGRA
	@ is the location of the Alpha channel, which
	@ we do not change. So adding 4 bytes to ABGR
	@ turns it into BGR. But since we are expected
	@ to return the address of the ABGR, we need
	@ to save it, call the BGRA routine, then
	@ restore the return address.
	@
	@ If not for that requirement, we would have
	@ just added 4 to R0 and fall through similarly
	@ to what we did with KOLIBA_Abgr32PixelToXyz,
	@ where we were able to change the input from
	@ R1 to R1+4 (the output of that routine is
	@ in the same R0 for both).

	push			{r0, lr}
	add				r0, r0, 4
	bl				KOLIBA_XyzToBgra32Pixel(PLT)
	pop				{r0, pc}

	.size	KOLIBA_XyzToAbgr32Pixel, .-KOLIBA_XyzToAbgr32Pixel

	.align	2

KOLIBA_XyzToBgra32Pixel:

	@ If there is no conversion function,
	@ just convert XYZ doubles to floats and return.
	@ Else, call the conversion function on each
	@ input and only then convert them.
	@
	@ So we can reuse the floating point registers
	@ for both, double and single, we shall convert
	@ y to green first, z to blue second, x to red
	@ last. And since we are converting XYZ to BGR,
	@ we store blue before green before red in the
	@ final output.

	cmp				r2, 0

	vldmiaeq.64		r1, {d0-d2}		@ X:Y:Z
	pushne			{r0, r4, r5, lr}
	vcvteq.f32.f64	s3, d1			@ S3 = G = Y (overwrites part of D1, OK)
	vpushne.f64		{d8-d10}
	vcvteq.f32.f64	s2, d2			@ S2 = B = Z (overwrites rest of D1, OK)
	vldmiane.64		r1, {d8-d10}
	vcvteq.f32.f64	s4, d0			@ S4 = R = X (overwrites part of D2, OK)
	movne			r4, r0
	vstmiaeq.32		r0, {s2-s4}		@ B:G:R
	vmovne.f64		d0, d9
	bxeq			lr

	mov				r5, r2
	blx				r2
	vcvt.f32.f64	s19, d0			@ overwrites part of D9, OK
	vmov.f64		d0, d10
	blx				r5
	vcvt.f32.f64	s18, d0			@ overwrites rest of D9, OK
	vmov.f64		d0, d8
	blx				r5
	vcvt.f32.f64	s20, d0			@ overwrites part of D10, OK
	vstmia.32		r4, {s18-s20}

	@ Hasta la Pasta!

	vpop.f64		{d8-d10}
	pop				{r0, r4-r5, pc}

	.size	KOLIBA_XyzToBgra32Pixel, .-KOLIBA_XyzToBgra32Pixel
