/*

	mkpslut.s - ARM EABI 32-bit version

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
	@		 R0  = address of output KOLIBA_SLUT
	@		 R1  = address of dim[3] dimensions m, n, o
	@		 R2  = address of ind[3] indices i, j, k
	@		 R3  = address of function to call
	@		[SP] = address of params for the function
	@
	@ On Exit:
	@
	@		 R0  = KOLIBA_SLUT or NULL

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_MakePartialSlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_MakePartialSlut, %function

KOLIBA_MakePartialSlut:

	@ Check for NULL pointers

	cmp				r3, 0
	cmpne			r2, 0
	cmpne			r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	mov				r12, sp
	bxeq			lr

	@ Check for invalid dimensions, but save regs first.

	push			{r0, r4-r9, lr}

	ldm				r2, {r7-r9}		@ R7 = i, R8 = j, R9 = k
	mov				r5, r3			@ R5 = function to call
	ldm				r1, {r1-r3}		@ R1 = m, R2 = n, R3 = o

	mov				r4, r0			@ R4 = start of sLut
	mov				r0, 255
	ldr				r6, [r12]		@ R6 = params

	add				r12, r7, 1
	vmov			s6, r7
	vmov			s9, r12
	add				r0, r0, 2		@ R0 = 257, R1-R3 must be < R0, R7-R9 must be < R1-R3
	add				r12, r8, 1
	vmov			s7, r8
	vmov			s10, r12
	add				r12, r9, 1
	vmov			s8, r9
	cmp				r1, r0
	vmov			s11, r12
	cmplt			r7, r1
	vmov			s0, r1
	cmplt			r2, r0
	cmplt			r8, r2
	vmov			s2, r2
	cmplt			r3, r0
	cmplt			r9, r3
	vmov			s4, r3
	movge			r0, 0
	vpushlt.f64		{d8-d13}

	@ Pop R1 instead of R0 because R0 needs to be NULL here.
	popge			{r1, r4-r9, lr}

	@ Prepare m, n, o divisors.

	vcvt.f64.u32	d0, s0			@ D0 = m
	vcvt.f64.u32	d1, s2			@ D1 = n
	vcvt.f64.u32	d2, s4			@ D2 = o

	vcvt.f64.u32	d8, s6			@ D8  = i
	vcvt.f64.u32	d9, s7			@ D9  = j
	vcvt.f64.u32	d10, s8			@ D10 = k

	vcvt.f64.u32	d11, s9			@ D11 = i+1
	vcvt.f64.u32	d12, s10		@ D12 = j+1
	vcvt.f64.u32	d13, s11		@ D13 = k+1

	vdiv.f64		d8, d8, d0		@ D8  = i / m
	vdiv.f64		d9, d9, d1		@ D9  = j / n
	vdiv.f64		d10, d10, d2	@ D10 = k / o

	vdiv.f64		d11, d11, d0	@ D11 = (i+1) / m
	vdiv.f64		d12, d12, d1	@ D12 = (j+1) / n
	vdiv.f64		d13, d13, d2	@ D13 = (k+1) / o

	vpush.f64		{d8-d10}		@ [i,j,k]/[m,n,o]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.black

	vstr.f64		d13, [sp, 16]	@ [i,j,k+1]/[m,n,o]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.blue

	vstr.f64		d10, [sp, 16]	@ [i,j+1,k]/[m,n,o]
	vstr.f64		d12, [sp, 8]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.green

	vstr.f64		d13, [sp, 16]	@ [i,j+1,k+1]/[m,n,o]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.cyan

	vstr.f64		d10, [sp, 16]	@ [i+1,j,k]/[m,n,o]
	vstr.f64		d9,  [sp, 8]
	vstr.f64		d11, [sp]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.red

	vstr.f64		d13, [sp, 16]	@ [i+1,j,k+1]/[m,n,o]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.magenta

	vstr.f64		d10, [sp, 16]	@ [i+1,j+1,k]/[m,n,o]
	vstr.f64		d12, [sp, 8]
	mov				r0, r4
	add				r4, r4, 8*3
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.yellow

	vstr.f64		d13, [sp, 16]	@ [i+1,j+1,k+1]/[m,n,o]
	mov				r0, r4
	mov				r1, sp
	mov				r2, r6
	blx				r5				@ sLut.white

	@ Hasta la Pasta!

	add				sp, sp, 8*3
	vpop.f64		{d8-d13}
	pop				{r0, r4-r9, pc}

	.size	KOLIBA_MakePartialSlut, .-KOLIBA_MakePartialSlut
