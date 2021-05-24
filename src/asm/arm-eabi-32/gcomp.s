/*

	gcomp.s - ARM EABI 32-bit version

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
	@		  R0    = address of output MATRIX
	@		  R1    = address of (const) RGB Rec
	@		  R2    = unsigned int channel {0-2}
	@
	@ On Exit:
	@
	@		 R0    = address output MATRIX or NULL

	.arch	armv6
	.text
	.align	2
	.global	KOLIBA_GrayComplementMatrix
	.syntax unified
	.arm
	.fpu 	vfp
	.type	KOLIBA_GrayComplementMatrix, %function

KOLIBA_GrayComplementMatrix:

	@ Save the channel number and the return address.

	push		{r2, lr}

	@ Write a grayscale matrix to the output.
	@ This will also check if there is a NULL
	@ pointer and whether the Rec is valid.

	bl			KOLIBA_GrayscaleMatrix

	@ If it returned a NULL, we just return.
	@ Also, if chanel > 2 return.
	@ Otherwise, restore the channel number
	@ and the return value.

	cmp			r0, 0
	pop			{r2, lr}
	movne		r1, 2
	bxeq		lr

	cmp			r2, r1
	bxhi		lr

	@ To save space, our table of defaults (below)
	@ is in reverse order. To load the right values
	@ we need to subtract the channel number from 2,
	@ so {0-2} becomes {2-0}.
	@
	@ At the same time, we also need to find the address
	@ of the matrix row we want to reset. There are four
	@ doubles in a row, 32 bytes.

	adr			r3, .Blue
	sub			r1, r1, r2
	add			r2, r0, r2, lsl 5	@ R2 = matrix row
	add			r1, r3, r1, lsl 3	@ R1 = address of defaults

	@ Load the defaults and store them in the matrix row.
	@ We do not need to set row.o to zero because the call
	@ to KOLIBA_GrayscaleMatrix already did that.

	vldmia.f64	r1, {d0-d2}
	vstmia.f64	r2, {d0-d2}

	@ Hasta la Pasta!

	bx			lr


	.align	3

.Blue:

	.double	0.0

.Green:

	.double	0.0

.Red:

	.double	1.0, 0.0, 0.0


	.size	KOLIBA_GrayComplementMatrix, .-KOLIBA_GrayComplementMatrix
