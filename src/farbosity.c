/*

	farbosity.c

	Copyright 2021 G. Adam Stanislav
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

#include <koliba.h>
#include <stdio.h>
#include <stdlib.h>

#define	CENTER	0.5
#define	WIDTH	1.0

int usage(int retval) {
	fprintf(stderr, "Usage: farbosity [-o] output [-c <center>] [-w <width]\n");
	return retval;
}

int main(unsigned int argc, char *argv[]) {
	KOLIBA_SLUT sLut;
	char *center = NULL;
	char *width = NULL;
	char *filename = NULL;
	unsigned int i;

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
#ifdef _WIN32
			|| (argv[i][0] == '/')
#endif
		) switch(argv[i][1]) {
			default:
				return usage(1);
				break;
			case 'c':
				if (argv[i][2]) center = &(argv[i][2]);
				else if (i < (argc-1)) {
					center = argv[++i];
				}
				else return usage(2);
				break;
			case 'o':
				if (argv[i][2]) filename = &(argv[i][2]);
				else if (i < (argc-1)) {
					filename = argv[++i];
				}
				else return usage(3);
				break;
			case 'w':
				if (argv[i][2]) width = &(argv[i][2]);
				else if (i < (argc-1)) {
					width = argv[++i];
				}
				else return usage(4);
				break;
		}
		else filename = argv[i];
	}

	if (filename == NULL) return usage(5);

	KOLIBA_WriteSlutToNamedFile(KOLIBA_ResetSlutWhite(KOLIBA_ResetSlutBlack(KOLIBA_SetFarbosity(&sLut, (center == NULL) ? CENTER : atof(center), (width == NULL) ? WIDTH : atof(width)))), filename);

	return 0;
}


