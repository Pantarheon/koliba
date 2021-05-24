/*

	sltconv.c

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

#include <stdio.h>
#include <string.h>
#include <koliba.h>

#define	version	"v.0.2"

const char notice[] = "sltconv, " version "\nCopyright 2019 G. Adam Stanislav\nAll rights reserved\n\n";

static const char cubehead[] = "TITLE \"%s\"\n"
	"DOMAIN_MIN 0 0 0\n"
	"DOMAIN_MAX 1 1 1\n"
	"LUT_%iD_SIZE 2\n";

static const char cubeline[] = "%.10g %.10g %.10g\n";

static const char matrixdesc[] = "# Converted from the matrix:\n#\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\n";

static const char cfltdesc[] = "# Converted from the color filter:\n#\n"
	"#\t[r,g,b] = [%.10g, %.10g, %.10g]\n"
	"#\tdensity =  %.10g\n#\n";

int usage(void) {
	fprintf(stderr, "Usage: sltconv input [output]\n");
	return 1;
}

int nofile(char *fname) {
	fprintf(stderr, "sltconv error: Could not open file \"%s\"\n", fname);
	return 2;
}

int invalid(FILE *f, char *fname) {
	fclose(f);
	fprintf(stderr, "sltconv error (\"%s\"): Unknown or invalid input format\n", fname);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *f;
	KOLIBA_SLUT2 sLut;
	union {
		KOLIBA_MATRIX2 m3x4;
		KOLIBA_CFLT2 cflt;
	} slt;
	double csum;
	unsigned char *ptr = (unsigned char *)&sLut;
	enum fd {slut, matrix, cflt} ftype;
	int is1d;

	fprintf(stderr, notice);

	if ((argc < 2) || (argc > 3))
		return usage();

	f = fopen(argv[1], "rb");
	if (f == NULL) return nofile(argv[1]);

	if (fread(ptr, 1, SLTCFILEHEADERBYTES, f) < SLTCFILEHEADERBYTES)
		return invalid(f, argv[1]);

	if (memcmp(ptr, KOLIBA_sLutHeader, SLTCFILEHEADERBYTES) == 0) {
		if ((fread(&sLut, 1, sizeof(sLut), f) != sizeof(sLut)) ||
		(KOLIBA_FixDoubles((double *)&sLut, sizeof(sLut) / sizeof(double)) == NULL) ||
		(!KOLIBA_CheckSlut(&sLut.sLut, sLut.checksum)) ||
		(KOLIBA_FixSlut(&sLut.sLut) == NULL))
			return invalid(f, argv[1]);
		else ftype = slut;
	}
	else if (memcmp(ptr, KOLIBA_m3x4Header, SLTCFILEHEADERBYTES) == 0) {
		if ((fread(&slt.m3x4, 1, sizeof(KOLIBA_MATRIX2), f) != sizeof(KOLIBA_MATRIX2)) ||
		(KOLIBA_FixDoubles((double *)&slt.m3x4, sizeof(KOLIBA_MATRIX2) / sizeof(double)) == NULL) ||
		(!KOLIBA_CheckMat(&slt.m3x4.mat, slt.m3x4.checksum)) ||
		(KOLIBA_ConvertMatrixToSlut(&sLut.sLut, &slt.m3x4.mat) == NULL) ||
		(KOLIBA_FixSlut(&sLut.sLut) == NULL))
			return invalid(f, argv[1]);
		else ftype = matrix;
	}
	else if (memcmp(ptr, KOLIBA_cFltHeader, SLTCFILEHEADERBYTES) == 0) {
		if ((fread(&slt.cflt, 1, sizeof(KOLIBA_CFLT2), f) != sizeof(KOLIBA_CFLT2)) ||
		(KOLIBA_FixDoubles((double *)&slt.cflt, sizeof(KOLIBA_CFLT2) / sizeof(double)) == NULL) ||
		(!KOLIBA_CheckCflt(&slt.cflt.cFlt, slt.cflt.checksum)) ||
		(KOLIBA_ConvertColorFilterToSlut(&sLut.sLut, &slt.cflt.cFlt) == NULL) ||
		(KOLIBA_FixSlut(&sLut.sLut) == NULL))
			return invalid(f, argv[1]);
		else ftype = cflt;
	}
	else return invalid(f, argv[1]);

	fclose(f);

	if (argc == 3) {
		f = fopen(argv[2], "wb");
		if (f == NULL) return nofile(argv[2]);
	}
	else f = stdout;

	is1d = KOLIBA_SlutIs1D(&sLut.sLut);

	fprintf(f, cubehead, argv[1], is1d ? 1 : 3);
	switch (ftype) {
		case matrix:
			fprintf(f, matrixdesc, slt.m3x4.mat.red.r, slt.m3x4.mat.red.g, slt.m3x4.mat.red.b, slt.m3x4.mat.red.o,
				slt.m3x4.mat.green.r, slt.m3x4.mat.green.g, slt.m3x4.mat.green.b, slt.m3x4.mat.green.o,
				slt.m3x4.mat.blue.r, slt.m3x4.mat.blue.g, slt.m3x4.mat.blue.b, slt.m3x4.mat.blue.o
			);
			break;
		case cflt:
			fprintf(f, cfltdesc, slt.cflt.cFlt.r, slt.cflt.cFlt.g, slt.cflt.cFlt.b, slt.cflt.cFlt.d);
			break;
		default:
			break;
	}
	fprintf(f, cubeline, sLut.sLut.black.r, sLut.sLut.black.g, sLut.sLut.black.b);

	if (!is1d) {
		fprintf(f, cubeline, sLut.sLut.red.r, sLut.sLut.red.g, sLut.sLut.red.b);
		fprintf(f, cubeline, sLut.sLut.green.r, sLut.sLut.green.g, sLut.sLut.green.b);
		fprintf(f, cubeline, sLut.sLut.yellow.r, sLut.sLut.yellow.g, sLut.sLut.yellow.b);
		fprintf(f, cubeline, sLut.sLut.blue.r, sLut.sLut.blue.g, sLut.sLut.blue.b);
		fprintf(f, cubeline, sLut.sLut.magenta.r, sLut.sLut.magenta.g, sLut.sLut.magenta.b);
		fprintf(f, cubeline, sLut.sLut.cyan.r, sLut.sLut.cyan.g, sLut.sLut.cyan.b);
	}
	fprintf(f, cubeline, sLut.sLut.white.r, sLut.sLut.white.g, sLut.sLut.white.b);
	fprintf(f, "\n## Converted from \"%s\" by sltconv, " version "\n\n", argv[1]);

	if (f != stdout) fclose(f);

	return 0;
}
