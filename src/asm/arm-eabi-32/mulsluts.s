/*

	mulsluts.s - ARM EABI 32-bit version

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
	@		R0 = output sLut
	@		R1 = (const) "multiplicand" sLut
	@		R2 = (const) "multiplier" sLut
	@
	@ On Output:
	@
	@		R0 = output sLut or NULL
	@
	@	This is not an actual multiplication,
	@	only a calculation somewhat analogous
	@	to matrix multiplication.
	@
	@	What we are actually doing is treating
	@	the vertices of the "multiplicand" as
	@	x,y,z inputs to the "multiplier" sLut
	@	(which we first convert to a fLut, of
	@	course) and treat the results as the
	@	corresponding vertices of the output sLut.
	@
	@	It is not as precise as matrix multiplication,
	@	which is the same as applying a function
	@	to the output of another function, i.e., f(g(x)),
	@	while this is applying a LUT to the output
	@	of another LUT. That is to say, interpolating
	@	values that are the results of another
	@	interpolation.
	@
	@	So, in theory, we can replace any chain of
	@	matrices with just one matrix derived by
	@	repeated matrix multiplication (in theory,
	@	or should I say in mathematics, because in
	@	computers we are always dealing with at least
	@	some rounding errors due to a finite precision
	@	of floating-point registers). But when applying
	@	a chain of LUTs, we are approximating recursively,
	@	losing some precision with every link in the chain.
	@
	@	Nevertheless, it is a way of producing
	@	nested LUTs, so to speak. And when using it
	@	for creating color effects, our final goal
	@	is producing something visually appealing,
	@	regardless of how mathematically precise it may
	@	or may not be.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_MultiplySluts
	.arch	 armv6
	.syntax unified
	.arm
	.fpu	vfp
	.equ	vertices,	(8*8)
	.equ	lut,	(3*8*8)
	.equ	xyz,	(3*8)
	.type	KOLIBA_MultiplySluts, %function

KOLIBA_MultiplySluts:

	@ If any of the three inputs is NULL, we cannot proceed.

	cmp			r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	bxeq		lr

	@ Save a few registers.

	push		{r4-r8, lr}
	mov			r4, r0			@ R4 = output
	mov			r5, r1			@ R5 = multiplicand

	@ Create enough local space for an array of vertices,
	@ a fLut and the temporary output (since it is possible
	@ for the final output to be overlapping an input or two).
	@
	@ Specifically, we will place the vertices at the TOS,
	@ the fLut below the vertices and the temporary sLut below
	@ the fLut. That will allow us to remove some of it
	@ on the fly when it is no longer needed.

	sub			sp, sp, 2*lut + vertices

	@ Point the vertices at the multiplier.

	mov			r0, sp
	mov			r1, r2
	bl			KOLIBA_SlutToVertices(PLT)

	@ Convert the multiplier to a fLut.

	mov			r1, r0
	add			r0, sp, vertices
	bl			KOLIBA_ConvertSlutToFlut(PLT)

	@ Figure out the flags. Also, we no longer
	@ need the vertices.

	add			sp, sp, vertices
	bl			KOLIBA_FlutFlags(PLT)
	mov			r6, r0			@ R6 = flags

	@ Get ready to loop eight times.

	add			r7, sp, lut		@ R7 = current XYZ output, R5 = current multiplicand (XYZ input)
	mov			r8, 8

.TheLoop:

	mov			r3, r6			@ R3 = flags
	mov			r2, sp			@ R2 = address of fLut
	mov			r1, r5			@ R1 = input  XYZ
	mov			r0, r7			@ R1 = output XYZ
	bl			KOLIBA_ApplyXyz(PLT)

	@ Are we there yet?

	subs		r8, r8, 1
	addne		r5, r5, xyz
	addne		r7, r7, xyz
	bne			.TheLoop

	@ We no longer need the fLut.

	add			sp, sp, lut

	@ Copy temporary output to the final output.

	mov			r0, r4
	vpop.f64	{d0-d7}
	vstmia.f64	r4!, {d0-d7}
	vpop.f64	{d0-d7}
	vstmia.f64	r4!, {d0-d7}
	vpop.f64	{d0-d7}
	vstmia.f64	r4, {d0-d7}

	@ Hasta la Pasta!

	pop			{r4-r8, pc}


	.size	KOLIBA_MultiplySluts, .-KOLIBA_MultiplySluts
