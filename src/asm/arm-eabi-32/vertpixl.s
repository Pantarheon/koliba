/*

	vertpixl.s - ARM EABI 32-bit version

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
	@		R0 = address of output float pixel
	@		R1 = address of (const) input double vertex
	@		R2 = number of each of the above

	.arch	armv6
	.text
	.syntax	unified
	.arm
	.fpu	vfp

	.align	3
	.global	KOLIBA_VertexToPixel
	.type	KOLIBA_VertexToPixel, %function

KOLIBA_VertexToPixel:

	@ If any of the inputs is NULL or 0, return NULL.

	cmp				r2, 0
	cmpne			r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Else, do the job!

	mov				r12, r0

.LV2PLoop:

	vldmia			r1!, {d0-d2}
	vcvt.f32.f64	s0, d0
	vcvt.f32.f64	s1, d1
	vcvt.f32.f64	s2, d2
	subs			r2, r2, 1
	vstmia			r12!, {s0-s2}
	bxeq			lr
	bl				.LV2PLoop

	.size	KOLIBA_VertexToPixel, .-KOLIBA_VertexToPixel




	@ On Input:
	@
	@		R0 = address of output double vertex
	@		R1 = address of (const) input float pixel
	@		R2 = number of each of the above

	.align	3
	.global	KOLIBA_PixelToVertex
	.type	KOLIBA_PixelToVertex, %function

KOLIBA_PixelToVertex:

	@ If any of the inputs is NULL or 0, return NULL.

	cmp				r2, 0
	cmpne			r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Else, do the job!

	mov				r12, r0

.LP2VLoop:

	vldmia			r1!, {s8-s10}
	vcvt.f64.f32	d0, s8
	vcvt.f64.f32	d1, s9
	vcvt.f64.f32	d2, s10
	subs			r2, r2, 1
	vstmia			r12!, {d0-d2}
	bxeq			lr
	bl				.LP2VLoop

	.size	KOLIBA_PixelToVertex, .-KOLIBA_PixelToVertex


	@ On Input:
	@
	@		R0 = address of (const) input double vertex
	@		R1 = address of (const) input float pixel

	.align	3
	.global	KOLIBA_PixelIsVertex
	.type	KOLIBA_PixelIsVertex, %function

KOLIBA_PixelIsVertex:

	@ If either of the inputs is NULL or 0, return FALSE.

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	@ Else, do the job!

	vldmia			r1, {s8-s10}
	vcvt.f64.f32	d0, s8
	vcvt.f64.f32	d1, s9
	vcvt.f64.f32	d2, s10

	vldmia			r0, {d4-d6}
	mov				r0, 0					@ Assume the test shows FALSE result.
	vcmp.f64		d0, d4
	vmrs			APSR_nzcv, fpscr
	bxne			lr
	vcmp.f64		d1, d5
	vmrs			APSR_nzcv, fpscr
	bxne			lr
	vcmp.f64		d2, d6
	vmrs			APSR_nzcv, fpscr
	moveq			r0, 1
	bx				lr
	
	.size	KOLIBA_PixelIsVertex, .-KOLIBA_PixelIsVertex

