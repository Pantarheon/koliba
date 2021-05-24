/*

	fltcube.s - ARM EABI 32-bit version

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
	@		 R0		= address of output KOLIBA_FLUT
	@		 R1		= address of (const) input cube (KOLIBA_RGB array)
	@		 R2		= address of (const) input dimension array, dim[3] = {m,n,o}
	@		 R3		= address of (const) input index array, ind[3] = {i,j,k}
	@
	@	Create a FLUT from a cube (or cuboid, actually, since each dimension
	@	can be of a different size), and indices of dimensions, i.e, i of m
	@	in the red (or X) dimension, j of o in the green (Y) dimension, and
	@	k of o in the blue (Z) dimension.
	@
	@	We do it by figuring out which RGB triplets in the cuboid correspond
	@	to a sLut for the given indices, assign their addresses to the
	@	KOLIBA_VERTICES structure of vertex pointers, and pass that to the
	@	KOLIBA_ConvertSlutToFlut code, which will compute the fLut values.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertCubeToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertCubeToFlut, %function

KOLIBA_ConvertCubeToFlut:

	@ Check for NULL pointers

	cmp			r3, 0
	cmpne		r2, 0
	cmpne		r1, 0
	moveq		r0, 0
	cmpne		r0, 0
	pushne		{r4-r8, lr}
	bxeq		lr

	@ Make sure indices are smaller than dimensions.
	@ This also checks for 0 dimensions since even if
	@ an index is 0, as long as it is lesser than the
	@ corresponding dimension, that dimension is > 0.

	ldm			r2, {r6-r8}			@ R6 = m, R7 = n, R8 = o
	ldm			r3, {r3-r5}			@ R3 = i, R4 = j, R5 = k

	cmp			r7, r4
	add			r7, r7, 1			@ R7 = n + 1
	cmphi		r6, r3
	add			r6, r6, 1			@ R6 = m + 1
	cmphi		r8, r5
	mla			r2, r5, r7, r4		@ R2 = k+(n+1) + j
	movls		r0, 0
	popls		{r4-r8, pc}

	@ Because in .cube red grows fastest and blue slowest,
	@ the pointer of cube[i][j][k] is P = (k*(n+1) + j)(m+1) + i.
	@
	@ And of course, we have to multiply it by 24, as there
	@ are three doubles in KOLIBA_RGB.

	mla			r2, r2, r6, r3		@ R2 = (k+(n+1)+j)(m+1) + i

	add			r6, r6, r6, lsl 1	@ R6 = 3(m+1)
	add			r2, r2, r2, lsl 1	@ R2 = 3P
	mul			r7, r6, r7			@ R7 = 3(m+1)(n+1)

	add			r1, r1, r2, lsl 3	@ R1 = cube + P * 24 = vert.black
	add			r2, r1, r7, lsl 3	@ R2 = cube + (P + (m+1)(n+1)) * 24 = vert.blue
	add			r3, r1, r6, lsl 3	@ R3 = cube + (P + (m+1)) * 24 = vert.green
	add			r4, r2, r6, lsl 3	@ R4 = cube + (P + (m+1)+(m+1)(n+1)) * 24 = vert.cyan

	@ Adding 24 is the same as adding red, given that red grows
	@ the fastest and that 24 bytes is the size of each KOLIBA_RGB.
	@
	@ black + red is red, blue + red is magenta, green + red is
	@ yellow, and cyan + red is white.

	add			r5, r1, 24			@ red
	add			r6, r2, 24			@ magenta
	add			r7, r3, 24			@ yellow
	add			r8, r4, 24			@ white

	push		{r1-r8}

	mov			r1, sp				@ R0 = fLut, R1 = vert
	bl			KOLIBA_ConvertSlutToFlut(PLT)

	add			sp, sp, 32
	pop			{r4-r8, pc}

	.size	KOLIBA_ConvertCubeToFlut, .-KOLIBA_ConvertCubeToFlut
