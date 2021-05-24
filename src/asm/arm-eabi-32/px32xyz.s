/*

	px32xyz.s - ARM EABI 32-bit version

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
	@		R0 = output double XYZ
	@		R1 = (const) input RGB(A)32PIXEL
	@		R2 = DBLCONV function, or NULL
	@
	@ On Output:
	@
	@		R0 = output XYZ

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Rgba32PixelToXyz
	.global	KOLIBA_Argb32PixelToXyz
	.arch armv6
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Rgba32PixelToXyz, %function
	.type	KOLIBA_Argb32PixelToXyz, %function

KOLIBA_Argb32PixelToXyz:

	add				r1, r1, 4

KOLIBA_Rgba32PixelToXyz:

	@ If there is no conversion function, just
	@ convert singles to doubles. Otherwise, run
	@ the doubles through the function afterwards.
	@
	@ We start seemingly at the end, i.e., convert
	@ blue to z first, otherwise we would overwrite
	@ the 32-bit values before converting them to
	@ 64 bits.

	cmp				r2, 0

	vldmiaeq.32		r1, {s0-s2}
	pushne			{r0, r4-r5, lr}
	vcvteq.f64.f32	d2, s2
	vpushne.f64		{d8-d10}
	vcvteq.f64.f32	d1, s1
	vldmiane.32		r1, {s16-s18}
	vcvteq.f64.f32	d0, s0
	movne			r4, r0
	vstmiaeq.64		r0, {d0-d2}
	vcvtne.f64.f32	d0, s18
	bxeq			lr

	mov				r5, r2
	blx				r2
	vmov.f64		d10, d0
	vcvt.f64.f32	d0, s17
	blx				r5
	vmov.f64		d9, d0
	vcvt.f64.f32	d0, s16
	blx				r5
	vmov.f64		d8, d0
	vstmia.64		r4, {d8-d10}

	@ Hasta la Pasta!

	vpop.f64		{d8-d10}
	pop				{r0, r4-r5, pc}

	.size	KOLIBA_Rgba32PixelToXyz, .-KOLIBA_Rgba32PixelToXyz
	.size	KOLIBA_Argb32PixelToXyz, .-KOLIBA_Argb32PixelToXyz
