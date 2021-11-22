/*

	eryslut.c

	Copyright 2019-2021 G. Adam Stanislav
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

#define USECLIB
#include <stdio.h>
#include <stdlib.h>
#include <koliba.h>

#define	version	"0.2"

int usage(void) {
	fprintf(stderr, "Usage: eryslut [-r red] [-g green] [-b blue] [-o filename] [-t|T]\n");
	return -1;
}

int main(int argc, char *argv[]) {
	KOLIBA_SLUT sLut;
	FILE *f;
	char *filename = "ery.sLut";
	unsigned int red   = 255;
	unsigned int green = 0;
	unsigned int blue  = 0;
	unsigned int i;
	char t = '\0';

	fprintf(stderr, "eryslut, v." version "\nCopyright 2021 G. Adam Stanislav\nAll rights reserved\n\n");

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
#ifdef _WIN32
			|| (argv[i][0] == '/')
#endif
			) switch (argv[i][1]) {
			case 'r':
				if (argv[i][2]) red = atoi(&(argv[i][2]));
				else if (++i < argc) {
					red = atoi(argv[i]);
				}
				else return usage();
				break;
			case 'g':
				if (argv[i][2]) green = atoi(&(argv[i][2]));
				else if (++i < argc) {
					green = atoi(argv[i]);
				}
				else return usage();
				break;
			case 'b':
				if (argv[i][2]) blue = atoi(&(argv[i][2]));
				else if (++i < argc) {
					blue = atoi(argv[i]);
				}
				else return usage();
				break;
			case 'o':
				if (argv[i][2]) filename = &(argv[i][2]);
				else if (++i < argc) {
					filename = argv[i];
				}
				else return usage();
				break;
			case 't':
				t = 1;
				break;
			case 'T':
				t = -1;
				break;
			default:
				return usage();
		}
		else filename = argv[i];
	}

	red   &= 255;
	green &= 255;
	blue  &= 255;

	KOLIBA_ResetSlut(&sLut);

	sLut.Red.r = KOLIBA_ByteDiv255[red];
	sLut.Red.g = KOLIBA_ByteDiv255[green];
	sLut.Red.b = KOLIBA_ByteDiv255[blue];

	f = fopen(filename, "wb");
	if (f == NULL) {
		fprintf(stderr, "eryslut: Could not create file '%s'.\n", filename);
		return -2;
	}
	i = (t == '\0') ? KOLIBA_WriteSlutToOpenFile(KOLIBA_ApplyErythropy(&sLut, &sLut), f) :
		KOLIBA_WriteSlttToOpenFile(KOLIBA_ApplyErythropy(&sLut, &sLut), f);
	if (t>0) fprintf(f, "\n## Created by eryslut (v." version ") -r %u -g %u -b %u\n", red, green, blue);
	if (i) fprintf(stderr, "eryslut: Failed to write to file '%s'.\n", filename);

	return i;
}

