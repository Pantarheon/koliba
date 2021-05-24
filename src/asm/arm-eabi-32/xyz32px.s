/*

	xyz32px.s - ARM EABI 32-bit version

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
	@		R0 = address of output RGB(A)32PIXEL
	@		R1 = address of (const) input XYZ
	@		R2 = address of double conversion function, or NULL
	@
	@ On Output:
	@
	@		R0 = output RGB(A)32PIXEL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_XyzToRgba32Pixel
	.global	KOLIBA_XyzToArgb32Pixel
	.arch armv6
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_XyzToRgba32Pixel, %function
	.type	KOLIBA_XyzToArgb32Pixel, %function

KOLIBA_XyzToArgb32Pixel:

	push			{r0, lr}
	add				r0, r0, 4
	bl				KOLIBA_XyzToRgba32Pixel(PLT)
	pop				{r0, pc}

	.size	KOLIBA_XyzToArgb32Pixel, .-KOLIBA_XyzToArgb32Pixel


KOLIBA_XyzToRgba32Pixel:

	@ If there is no conversion function,
	@ just convert XYZ doubles to floats and return.
	@ Else, call the conversion function on each
	@ input and only then convert them.

	cmp				r2, 0

	vldmiaeq.64		r1, {d0-d2}
	pushne			{r0, r4, r5, lr}
	vcvteq.f32.f64	s0, d0
	vpushne.f64		{d8-d10}
	vcvteq.f32.f64	s1, d1
	vldmiane.64		r1, {d8-d10}
	vcvteq.f32.f64	s2, d2
	movne			r4, r0
	vstmiaeq.32		r0, {s0-s2}
	vmovne.f64		d0, d8
	bxeq			lr

	mov				r5, r2
	blx				r2
	vcvt.f32.f64	s16, d0
	vmov.f64		d0, d9
	blx				r5
	vcvt.f32.f64	s17, d0
	vmov.f64		d0, d10
	blx				r5
	vcvt.f32.f64	s18, d0
	vstmia.32		r4, {s16-s18}

	@ Hasta la Pasta!

	vpop.f64		{d8-d10}
	pop				{r0, r4-r5, pc}

	.size	KOLIBA_XyzToRgba32Pixel, .-KOLIBA_XyzToRgba32Pixel
