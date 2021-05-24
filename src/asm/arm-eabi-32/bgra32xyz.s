/*

	bgra32xyz.s - ARM EABI 32-bit version

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
	@		R1 = (const) input BGR(A)32PIXEL
	@		R2 = DBLCONV function, or NULL
	@
	@ On Output:
	@
	@		R0 = output XYZ

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_Bgra32PixelToXyz
	.global	KOLIBA_Abgr32PixelToXyz
	.arch armv6
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_Bgra32PixelToXyz, %function
	.type	KOLIBA_Abgr32PixelToXyz, %function

KOLIBA_Abgr32PixelToXyz:

	@ Just skip over the "A" to the "bgr"
	@ and fall through.

	add				r1, r1, 4

KOLIBA_Bgra32PixelToXyz:

	@ If there is no conversion function, just
	@ convert singles to doubles. Otherwise, run
	@ the doubles through the function afterwards.
	@
	@ The source has blue before green before red.
	@ So we load it starting where the x result
	@ shall go, followed by z, then y. Otherwise
	@ we would overwrite parts of the source
	@ before converting it.
	@
	@ Yet, we do it all without having to push any
	@ FPU register we do not push in the RGBA version.
	@ That allows us to write Vegas Pro plug-ins
	@ that require no extra time over the RGBA variety.
	@ Well, at least in theory, since Vegas does not
	@ run on ARM processors at this time.

	cmp				r2, 0

	vldmiaeq.32		r1, {s2-s4}		@ B:G:R
	pushne			{r0, r4-r5, lr}
	vcvteq.f64.f32	d0, s4			@ D0 = X = S4 = R (overwrites S0-S1, OK)
	vpushne.f64		{d8-d10}
	vcvteq.f64.f32	d2, s2			@ D2 = Z = S2 = B (overwrites S4-S5, OK)
	vldmiane.32		r1, {s18-s20}
	vcvteq.f64.f32	d1, s3			@ D1 = Y = S3 = G (overwrites S2-S3, OK)
	movne			r4, r0
	vstmiaeq.64		r0, {d0-d2}
	vcvtne.f64.f32	d0, s20
	bxeq			lr

	mov				r5, r2
	blx				r2
	vmov.f64		d8, d0			@ overwrites S16-S17 with X=R, OK
	vcvt.f64.f32	d0, s18
	blx				r5
	vmov.f64		d10, d0			@ overwrites S20-S21 with Z=B, OK
	vcvt.f64.f32	d0, s19
	blx				r5
	vmov.f64		d9, d0			@ overwrites S18-S19 with Y=G, OK
	vstmia.64		r4, {d8-d10}

	@ Hasta la Pasta!

	vpop.f64		{d8-d10}
	pop				{r0, r4-r5, pc}

	.size	KOLIBA_Bgra32PixelToXyz, .-KOLIBA_Bgra32PixelToXyz
	.size	KOLIBA_Abgr32PixelToXyz, .-KOLIBA_Abgr32PixelToXyz
