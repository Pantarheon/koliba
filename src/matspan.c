/*

	matspan.c

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

#define USECLIB
#include <stdio.h>
#include <string.h>
#include <koliba.h>

#define	VERSION	"v1.1.0"

#define	BINARY	0
#define	TEXTUAL	1
#define	DECIMAL	-1
#define	isbinary(b)	(b==0)
#define	istextual(t)	(t>0)
#define	isdecimal(d)	(d<0)

const char decimal[] = 	"%.10g %.10g %.10g %.10g\n"
	"%.10g %.10g %.10g %.10g\n"
	"%.10g %.10g %.10g %.10g\n"
	"\n";


int usage(int retval) {
	fprintf((retval) ? stderr : stdout, "Usage: matspan [[-o] output] options ...\n\n"
		"\t-g  gain (default = 0)\n"
		"\t-l  lift (default = 1)\n"
		"\t-rg red gain\n"
		"\t-rl red lift\n"
		"\t-gg green gain\n"
		"\t-gl green lift\n"
		"\t-bg blue gain\n"
		"\t-bl blue lift\n"
		"\t-s = binary output (default)\n"
		"\t-t = textual output\n"
		"\t-c = decimal output\n\n"
	);
	return retval;
}

int main(unsigned int argc, char *argv[]) {
	KOLIBA_MATRIX mat;
	FILE *f;
	KOLIBA_RGB a  = {1.0, 1.0, 1.0};	// a = gain
	KOLIBA_RGB b  = {0.0, 0.0, 0.0};	// b = lift
	unsigned int i;
	char *output  = NULL;
	char *strptr;
	int   otype = BINARY;

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
			case 'b':
				if (argv[i][2] == 'g') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					a.b = atof(strptr);
				}
				else if (argv[i][2] == 'l') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					b.b = atof(strptr);
				}
				else return usage(2);
				break;
			case 'g':
				if (argv[i][2] == 'g') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					a.g = atof(strptr);
				}
				else if (argv[i][2] == 'l') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					b.g = atof(strptr);
				}
				else {
					if (argv[i][2]) strptr = argv[i]+2;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					a.r = atof(strptr);
					a.g = a.r;
					a.b = a.r;
				}
				break;
			case 'l':
				if (argv[i][2]) strptr = argv[i]+2;
				else if (++i < argc) strptr = argv[i];
				else return usage(2);
				b.r = atof(strptr);
				b.g = b.r;
				b.b = b.r;
				break;
			case 'o':
				if (argv[i][2]) output = argv[i]+2;
				else if (++i < argc) output = argv[i];
				else return usage(3);
				break;
			case 'r':
				if (argv[i][2] == 'g') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					a.r = atof(strptr);
				}
				else if (argv[i][2] == 'l') {
					if (argv[i][3]) strptr = argv[i]+3;
					else if (++i < argc) strptr = argv[i];
					else return usage(2);
					b.r = atof(strptr);
				}
				else return usage(2);
				break;
			case 's':
				otype = BINARY;
				break;
			case 't':
				otype = TEXTUAL;
				break;
			case 'c':
				otype = DECIMAL;
				break;
		}
		else if (output == NULL) output = argv[i];
		else return usage(5);
	}

	fprintf(stderr, "matspan, " VERSION "\nCopyright 2021 G. Adam Stanislav\nAll rights reserved\n\n");

	KOLIBA_MatrixSpan(&mat, &a, &b);

	if (output == NULL) f = KOLIBA_StdOut();
	else if ((f = KOLIBA_OpenToWrite(output)) == NULL) {
		fprintf(stderr, "matspan: Could not open output file '%s'.\n", output);
		return 6;
	}

	i = (isbinary(otype)) ? KOLIBA_WriteMatrixToOpenFile(&mat, f) :
		(istextual(otype)) ? KOLIBA_WriteM34tToOpenFile(&mat, f) :
		(KOLIBA_Fprintf(f, decimal, mat.Red.r, mat.Red.g, mat.Red.b, mat.Red.o,
			mat.Green.r, mat.Green.g, mat.Green.b, mat.Green.o,
			mat.Blue.r, mat.Blue.g, mat.Blue.b, mat.Blue.o) <= 0);

	if (output) KOLIBA_Close(f);

	if (i) {
		fprintf(stderr, "matspan: Could not write the output matrix.\n");
		return 7;
	}

	return 0;
}
