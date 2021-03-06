/*

	matconv.c

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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <koliba.h>

#define	VERSION	"v1.1.0.3"

int usage(int retval) {
	fprintf((retval) ? stderr : stdout, "Usage: matconv [-i] input [[-o] output] [-e efficacy] [-n|N] [-t|s|f|F]\n");
	return retval;
}

int main(unsigned int argc, char *argv[]) {
	KOLIBA_MATRIX mat, *m;
	FILE *f;
	char *ptr;
	unsigned int i;
	double efficacy = 1.0;
	char *input     = NULL;
	char *output    = NULL;
	bool  normalize = false;
	bool  textual   = false;
	bool  svg       = false;

	if (argc < 2) return usage(1);

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
#ifdef	_WIN32
			|| (argv[i][0] == '/')
#endif
		) switch (argv[i][1]) {
			case 'h':
#ifdef _WIN32
			case '?':
#endif
				return usage(0);
				break;
			case 'e':
				if (argv[i][2]) ptr = argv[i]+2;
				else if (++i < argc) ptr = argv[i];
				else return usage(3);
				efficacy = atof(ptr);
				break;
			case 'F':
				svg     = true;
				textual = true;
				break;
			case 'f':
				svg     = true;
				textual = false;
				break;
			case 'i':
				if (argv[i][2]) input = argv[i]+2;
				else if (++i < argc) input = argv[i];
				else return usage(3);
				break;
			case 'N':
				normalize = false;
				break;
			case 'n':
				normalize = true;
				break;
			case 'o':
				if (argv[i][2]) output = argv[i]+2;
				else if (++i < argc) output = argv[i];
				else return usage(3);
				break;
			case 's':
				textual = false;
				svg     = false;
				break;
			case 't':
				textual = true;
				svg     = false;
				break;
		}
		else if (input  == NULL) input  = argv[i];
		else if (output == NULL) output = argv[i];
		else return usage(4);
	}

	fprintf(stderr, "matconv, " VERSION "\nCopyright 2021 G. Adam Stanislav\nAll rights reserved\n\n");

	if (input == NULL) return usage(5);
	if ((f = KOLIBA_OpenToRead(input)) == NULL) {
		fprintf(stderr, "matconv: Could not open input file '%s'.\n", input);
		return 5;
	}

	m = KOLIBA_ReadMatrixFromCompatibleOpenFile(&mat, f, NULL);
	KOLIBA_Close(f);

	if (m == NULL) {
		fprintf(stderr, "matconv: '%s' does not appear to be a supported matrix file.\n", input);
		return 6;
	}

	if (output == NULL) f = KOLIBA_StdOut();
	else if ((f = KOLIBA_OpenToWrite(output)) == NULL) {
		fprintf(stderr, "matconv: Could not create output file '%s'.\n", output);
		return 7;
	}

	if (efficacy != 1.0) KOLIBA_MatrixEfficacy(m, m, efficacy);

	if (normalize) {
		KOLIBA_NormalizeMatrixRow(&mat.Red, 1);
		KOLIBA_NormalizeMatrixRow(&mat.Green, 1);
		KOLIBA_NormalizeMatrixRow(&mat.Blue, 1);
	}

	KOLIBA_FixMatrix(&mat);

	if (svg) {
		i = KOLIBA_Fprintf(f, (textual) ?
			"<filter id=\"Matrix\" filterUnits=\"objectBoundingBox\" x=\"0%%\" y=\"0%%\" width=\"100%%\" height=\"100%%\">\n"
			"\t<feColorMatrix in=\"SourceGraphic\" type=\"matrix\" values=\"%g %g %g %g 0  %g %g %g %g 0  %g %g %g %g 0  0 0 0 1 0\"/>\n"
			"</filter>\n"
			: "<feColorMatrix in=\"SourceGraphic\" type=\"matrix\" values=\"%g %g %g %g 0  %g %g %g %g 0  %g %g %g %g 0  0 0 0 1 0\"/>\n",
			mat.Red.r,   mat.Red.g,   mat.Red.b,   mat.Red.o,
			mat.Green.r, mat.Green.g, mat.Green.b, mat.Green.o,
			mat.Blue.r,  mat.Blue.g,  mat.Blue.b,  mat.Blue.o
		) <= 0;
	}
	else if (textual) {
		i = KOLIBA_WriteM34tToOpenFile(&mat, f);
		KOLIBA_Fprintf(f, "\n# Created with matconv " VERSION "\n");
	}
	else i = KOLIBA_WriteMatrixToOpenFile(&mat, f);
	if (output != NULL) KOLIBA_Close(f);
	if (i) {
		fprintf(stderr, "matconv: Failed to write matrix to output.\n");
		return 8;
	}

	return 0;
}
