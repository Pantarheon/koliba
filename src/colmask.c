/*

	colmask.c

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

#if !defined(NULL)
	#define	NULL	((void*)0)
#endif

KLBDC KOLIBA_RGB * KOLIBA_ColorMask(KOLIBA_RGB * rgbout, const KOLIBA_RGB * const rgbin, const void * const params) {
	const KOLIBA_COLORMASK * cm = (KOLIBA_COLORMASK *)params;
	KOLIBA_RGB rgb;

	if ((rgbout == NULL) || (rgbin == NULL)) return NULL;

	if ((cm == NULL) || (cm->speed > 6) || (cm->channel > 2) || (cm->low > cm->high)) {
		rgbout->r = rgbin->r;
		rgbout->g = rgbin->g;
		rgbout->b = rgbin->b;
	}
	else {
		double max, min, mid, d;
		unsigned int ok = 1;

		if (cm->flags & KOLIBA_COLORMASK_NEGATIVE) {
			rgb.r = 1.0 - rgbin->r;
			rgb.g = 1.0 - rgbin->g;
			rgb.b = 1.0 - rgbin->b;
		} else {
			rgb.r = rgbin->r;
			rgb.g = rgbin->g;
			rgb.b = rgbin->b;
		}

		switch (cm->channel) {
			case 0:
				if ((rgb.r > rgb.g) && (rgb.r > rgb.b)) {
					max = rgb.r;
					if (rgb.g >= rgb.r) {mid = rgb.g; min = rgb.b;}
					else {mid = rgb.b; min = rgb.g;}
				}
				else ok = 0;
				break;
			case 1:
				if ((rgb.g > rgb.r) && (rgb.g > rgb.b)) {
					max = rgb.g;
					if (rgb.r >= rgb.b) {mid = rgb.r; min = rgb.b;}
					else {mid = rgb.b; min = rgb.r;}
				}
				else ok = 0;
				break;
			case 2:
				if ((rgb.b > rgb.r) && (rgb.b > rgb.g)) {
					max = rgb.b;
					if (rgb.r >= rgb.g) {mid = rgb.r; min = rgb.g;}
					else {mid = rgb.g; min = rgb.r;}
				}
				else ok = 0;
				break;
		}
		if (ok == 0) d = 0.0;
		else {
			if (cm->flags & KOLIBA_COLORMASK_CHROMINANCE) d = max - min;
			else d = max - mid;

			// Only go relative if max is positive. If it is 0, you would
			// cause an error; if it is negative, you would get nonsense.
			if ((cm->flags & KOLIBA_COLORMASK_RELATIVE) && (max > 0.0)) d /= max;

			if (d >= cm->high) d = 1.0;
			else if (d <= cm->low) d = 0.0;
			else d = (d - cm->low) / (cm->high - cm->low);
			// Because of the first two conditions, you will never get to
			// the third option when high = low, so no division by 0.

			switch (cm->speed) {
				case 0:
					d *= d;
				case 1:
					d *= d;
				case 2:
					d *= d;
				default:
					break;
				case 6:
					d = sqrt(d);
				case 5:
					d = sqrt(d);
				case 4:
					d = sqrt(d);
					break;
			}
		}

		if (cm->flags & KOLIBA_COLORMASK_INVERT) d = 1.0 - d;

		rgbout->r = d;
		rgbout->g = d;
		rgbout->b = d;
	}

	return rgbout;
}
