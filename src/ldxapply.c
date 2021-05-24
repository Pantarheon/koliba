/*

	ldxapply.c

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

#include "koliba.h"
#include <math.h>
#include <string.h>

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

#define	LDIR	(lumidux->ldirection)
#define	SDIR	(lumidux->sdirection)

static double speed(double d, unsigned int s) {
	// Since 0 < d < 1, we can safely produce
	// any of these results without checking
	// the bounds of d.
	//
	// And yes, I know certain purists blow up
	// and turn into tar and feathers when they
	// see a case that does not end with a break.

	switch (s) {
	case 0:
		d *= d;
	case 1:
		d *= d;
	case 2:
		d *= d;
	default:
		return d;
		break;
	case 6:
		d = sqrt(d);
	case 5:
		d = sqrt(d);
	case 4:
		return sqrt(d);
		break;
	}
}
static double lumi(const KOLIBA_XYZ * const foreground, const KOLIBA_XYZ * const background, const KOLIBA_LDX * const lumidux, const KOLIBA_RGB * const rec) {
	double lum, lo, hi;
	const KOLIBA_XYZ *pixel = (lumidux->lbase == 0) ? background : foreground;

	lum = pixel->x * rec->r + pixel->y * rec->g + pixel->z * rec->b;

	if (lumidux->llow <= lumidux->lhigh) {
		lo = lumidux->llow;
		hi = lumidux->lhigh;
	}
	else {
		lo = lumidux->lhigh;
		hi = lumidux->llow;
	}

	if (lum <= lo) lum = 0.0;
	else if (lum >= hi) lum = 1.0;
	else lum = (lum - lo) / (hi - lo);


	return speed((LDIR == 0) ? lum : 1.0 - lum, (unsigned int)lumidux->lspeed);
}

static double satu(const KOLIBA_XYZ * const foreground, const KOLIBA_XYZ * const background, const KOLIBA_LDX * const lumidux) {
	double max, min, sat, lo, hi;
	const KOLIBA_XYZ *pixel = (lumidux->sbase == 0) ? background : foreground;

	max = (pixel->x > pixel->y) ? pixel->x : pixel->y;
	if (pixel->z > max) max = pixel->z;

	min = (pixel->x < pixel->y) ? pixel->x : pixel->y;
	if (pixel->z < min) min = pixel->z;

	sat = max - min;
	// N.B. max can be 0 even if sat is not. It can happen
	//		when min is negative.
	if ((lumidux->schroma == 0) && (max != 0.0)) sat /= max;

	if (lumidux->slow <= lumidux->shigh) {
		lo = lumidux->slow;
		hi = lumidux->shigh;
	}
	else {
		lo = lumidux->shigh;
		hi = lumidux->slow;
	}

	if (sat <= lo) sat = 0.0;
	else if (sat >= hi) sat = 1.0;
	else sat = (sat - lo) / (hi - lo);

	return speed((SDIR == 0) ? sat : 1.0 - sat, (unsigned int)lumidux->sspeed);
}


// Apply Lumidux to a pixel.

KLBDC KOLIBA_XYZ * KOLIBA_ApplyLumidux(
	KOLIBA_XYZ * output,					// output xyz/pixel
	const KOLIBA_XYZ * const foreground,	// foreground xyz/pixel
	const KOLIBA_XYZ * const background,	// background xyz/pixel
	const KOLIBA_LDX * const lumidux,		// Lumidux to apply
	const KOLIBA_RGB * const rec			// May be NULL
) {
	double efficacy;

//	if ((output == NULL) || (foreground == NULL) || (background == NULL) || (lumidux == NULL)) return NULL;

	if (LDIR >= 0) {
		efficacy = (SDIR < 0) ? lumi(foreground, background, lumidux, (rec == NULL) ? &KOLIBA_Rec2020 : rec) : sqrt(lumi(foreground, background, lumidux, (rec == NULL) ? &KOLIBA_Rec2020 : rec) * satu(foreground, background, lumidux));
	}
	else if (SDIR >= 0) {
		efficacy = satu(foreground, background, lumidux);
	}
	else {
		return (KOLIBA_XYZ *)memcpy(output, foreground, sizeof(KOLIBA_XYZ));
	}
	if (lumidux->mask == 0) return (KOLIBA_XYZ *)KOLIBA_Interpolate((double *)output, (double *)foreground, efficacy, (double *)background, sizeof(KOLIBA_XYZ)/sizeof(double));
	else {
		output->x = efficacy;
		output->y = efficacy;
		output->z = efficacy;
		return output;
	}
}
