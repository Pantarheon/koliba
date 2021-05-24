/*

	xtrctch.c

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

// Extract a channel to all channels
KLBDC const KOLIBA_MATRIX KOLIBA_Redx = {
	{1.0, 0.0, 0.0, 0.0},
	{1.0, 0.0, 0.0, 0.0},
	{1.0, 0.0, 0.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Greenx = {
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Bluex = {
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0, 0.0}
};

// Separate a channel away from the rest
KLBDC const KOLIBA_MATRIX KOLIBA_Reds = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Greens = {
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Blues = {
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0}
};

// Remove a channel
KLBDC const KOLIBA_MATRIX KOLIBA_Nored = {
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Nogreen = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0}
};

KLBDC const KOLIBA_MATRIX KOLIBA_Noblue = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0}
};

