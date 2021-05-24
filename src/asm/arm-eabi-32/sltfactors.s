/*

	sltfactors.s - ARM EABI 32-bit version

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
	@		R0 = address of output KOLIBA_FLUT (F)
	@		R1 = address of (const) input KOLIBA_VERTICES(V)
	@
	@ On Exit:
	@
	@		R0 = address of KOLIBA_FLUT or NULL
	@
	@	We want to allow the two structures (input V and output F)
	@	to overlap. We do not have enough registers to hold
	@	everything and do the arithmetic at the same time, so
	@	we store all the computed vertices on the stack. At the end,
	@	we copy the stack to the output.
	@
	@	And yes, all we are doing here is basic arithmetic, just
	@	a simple addition and subtraction of 3D vectors.

	.arch armv6
	.text
	.align	2
	.global	KOLIBA_ConvertSlutToFlut
	.syntax unified
	.arm
	.fpu vfp
	.type	KOLIBA_ConvertSlutToFlut, %function

KOLIBA_ConvertSlutToFlut:

	cmp				r1, 0
	moveq			r0, 0
	cmpne			r0, 0
	bxeq			lr

	push			{r4, r5, r6, r7, r8, lr}
	vpush			{d8-d14}
	sub				sp, sp, 3*8*8

	ldmia			r1, {r2, r3, r4, r5, r6, r7, r8, r12}

	/*
			R2  = address of black   vertex
			R3  = address of blue    vertex
			R4  = address of green   vertex
			R5  = address of cyan    vertex
			R6  = address of red     vertex
			R7  = address of magenta vertex
			R8  = address of yellow  vertex
			R12 = address of white   vertex
	*/

	@ F.black = V.black

	mov				r1, sp
	vldmia.f64		r2, {d0-d2}			@ {D0-D2} = F.black = V.black
	vstmia.f64		r1!, {d0-d2}		@ F.black stored

	@ F.red = V.red - V. black

	vldmia.f64		r6, {d3-d5}			@ {D3-D5} = V.red
	vsub.f64		d6, d3, d0
	vsub.f64		d7, d4, d1
	vsub.f64		d8, d5, d2
	vstmia.f64		r1!, {d6-d8}		@ F.red stored

	@ F.green = V.green - V.black

	vldmia.f64		r4, {d6-d8}			@ {D6-D8} = V.green
	vsub.f64		d9, d6, d0
	vsub.f64		d10, d7, d1
	vsub.f64		d11, d8, d2
	vstmia.f64		r1!, {d9-d11}		@ F.green stored

	@ F.blue = V.blue - V.black

	vldmia.f64		r3, {d12-d14}

	vadd.f64		d9, d3, d9
	vadd.f64		d10, d4, d10

	vsub.f64		d0, d12, d0
	vsub.f64		d1, d13, d1
	vsub.f64		d2, d14, d2			@ {D0-D2} = F.blue = V.blue - V.black

	vadd.f64		d11, d5, d11		@ {D9-D11} = F.green + V.red = V.green + V.red - V.black

	vstmia.f64		r1!, {d0-d2}		@ F.blue stored

	@ F.yellow = V.black + V.yellow - V.green - V.red =
	@ = V.yellow - (V.green + V.red - V.black) = V.yellow - (F.green + V.red)

	vldmia.f64		r8, {d12-d14}		@ {D12-D14} = V.yellow
	vsub.f64		d9, d12, d9
	vsub.f64		d10, d13, d10
	vsub.f64		d11, d14, d11		@ {D9-D11} = F.yellow
	vstmia.f64		r1!, {d9-d11}		@ F.yellow stored

	@ F.magenta = V.black + V.magenta - V.blue - V.red =
	@ V.magenta - (V.blue - V.black) - V.red = V.magenta - F.blue - V.red

	vldmia.f64		r7, {d9-d11}		@ {D9-D11} = V.magenta
	vadd.f64		d12, d9, d12
	vsub.f64		d9, d9, d0
	vadd.f64		d13, d10, d13
	vsub.f64		d10, d10, d1
	vadd.f64		d14, d11, d14		@ {D12-D14} = V.yellow + V.magenta
	vsub.f64		d11, d11, d2
	vsub.f64		d9, d9, d3
	vsub.f64		d10, d10, d4
	vsub.f64		d11, d11, d5		@ {D9-D11} = F.magenta

	vsub.f64		d12, d3, d12
	vsub.f64		d13, d4, d13
	vsub.f64		d14, d5, d14		@ {D12-D14} = V.red - (V.yellow + V.magenta)

	vstmia.f64		r1!, {d9-d11}		@ F.magenta stored

	@ F.cyan = V.black + V.cyan - V.blue - V.green =
	@ = V.cyan - (V.blue - V.black) - V.green

	vldmia.f64		r5, {d9-d11}		@ {D9-D11} = V.cyan

	vsub.f64		d9, d9, d0
	vsub.f64		d10, d10, d1
	vsub.f64		d11, d11, d2

	vsub.f64		d9, d9, d6
	vsub.f64		d10, d10, d7
	vsub.f64		d11, d11, d8		@ {D9-D11} = F.cyan

	@ F.white = V.white + V.red + V.green + V.blue - V.black - V.cyan - V.magenta - V.yellow =
	@ = V.white + (V.red - (V.magenta + V.yellow)) - (V. cyan - (V.blue - V.black) - V.green) =
	@ = V.white + (V.red - (V.magenta + V.yellow)) - F.cyan

	vldmia.f64		r12, {d0-d2}		@ {D0-D2} = V.white

	vadd.f64		d0, d12, d0
	vadd.f64		d1, d13, d1
	vadd.f64		d2, d14, d2

	vsub.f64		d12, d0, d9
	vsub.f64		d13, d1, d10
	vsub.f64		d14, d2, d11		@ {D12-D14} = F.white
	vstmia.f64		r1, {d9-d14}		@ F.cyan and F.white stored

	mov				r5, r0
	mov				r0, sp
	bl				KOLIBA_FixSlut(PLT)
	mov				r0, r5

	@ Copy the first four factors from the stack to the output.

	vldmia.f64		sp!, {d0-d11}
	vstmia.f64		r5!, {d0-d11}

	@ Copy the remaining four factors from the stack to the output.

	vldmia.f64		sp!, {d0-d11}
	vstmia.f64		r5, {d0-d11}

	/*
		We can use F to interpolate any [r,g,b] pixel,
		or any [x,y,z] value, into V by the following
		algorithm:

		V(r,g,b) = 

			V.black   * (1 - r - g - b + rg + rb + gb + rg - rgb) +
			V.blue    * (            b      - rb - gb      + rgb) +
			V.green   * (        g     - rg      - gb      + rgb) +
			V.cyan    * (                        + gb      - rgb) +
			V.red     * (    r         - rg - rb           + rgb) +
			V.magenta * (                     rb           - rgb) +
			V.yellow  * (                rg                - rgb) +
			V.white   * (                                    rgb)

									=

	1   * V.black +
	r   * (V.red     - V.black) +
	g   * (V.green   - V.black) +
	b   * (V.blue    - V.black) +
	rg  * (V.yellow  + V.black - V.red   - V.green) +
	rb  * (V.magenta + V.black - V.red   - V.blue) +
	gb  * (V.cyan    + V.black - V.green - V.blue) +
	rgb * (V.white   - V.black + V.red   + V.green + V.blue - V.cyan - V.magenta - V.yellow)

									=

		F(r,g,b) =

			F.black        +
			F.red     * r  +
			F.green   * g  +
			F.blue    * b  +
			F.yellow  * rg +
			F.magenta * rb +
			F.cyan    * gb +
			F.white   * rgb

		So, if we only need to apply a sLut to one [r,g,b] or [x,y,z]
		triad, using the first algorithm (V) is probably faster because
		it avoids computing F.
		
		But if applying it to more than one such value (potentially many
		millions of them when processing frames of video), calculating F
		first and using the second algorithm (F) is considerably faster
		than using the V algorithm.

		Either way, V(r,g,b) = F(r,g,b), as both algorithms compute the
		same result for the same [r,g,b].

		In both cases, r, g, and b are treated as scalars and rg, rb, gb,
		and rgb are their scalar multiples, i.e., rg = r * g, etc.
	*/

	@ Hasta la pasta!

	vpop			{d8-d14}
	pop				{r4, r5, r6, r7, r8, pc}

	.size	KOLIBA_ConvertSlutToFlut, .-KOLIBA_ConvertSlutToFlut
