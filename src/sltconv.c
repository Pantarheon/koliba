/*

	sltconv.c

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
#include <string.h>
#include <koliba.h>

#define	version	"v.0.5.3"

typedef	union {
	KOLIBA_CHROMAT chrm;
	KOLIBA_MATRIX m3x4;
	KOLIBA_CFLT cflt;
} saluti;

typedef enum fd {slut, matrix, cflt, chromat} FTYPE;

const char notice[] = "sltconv, " version "\nCopyright 2019-2021 G. Adam Stanislav\nAll rights reserved\n\n";

static const char cubehead[] = "TITLE \"%s\"\n"
	"DOMAIN_MIN 0 0 0\n"
	"DOMAIN_MAX 1 1 1\n"
	"LUT_%iD_SIZE 2\n";

static const char cubeline[] = "%.10g %.10g %.10g\n";

static const char withefficacy[] = "# With %g%% efficacy.\n#\n";

static const char matrixdesc[] = "# Converted from the matrix:\n#\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\t%.10g %.10g %.10g %.10g\n"
	"#\n";

static const char chrmdesc[] = "# Converted from the chromatic matrix:\n#\n"
	"#\tRGB Model:\n#\n"
	"#\t\tred        = %.10g\n"
	"#\t\tgreen      = %.10g\n"
	"#\t\tblue       = %.10g\n#\n"
	"#\tChroma:\n#\n"
	"#\t\tangle      = %.10g degrees\n"
	"#\t\tmagnitude  = %.10g\n"
	"#\t\tsaturation = %.10g\n"
	"#\t\tblack      = %.10g\n"
	"#\t\twhite      = %.10g\n"
	"#\n";

static const char cfltdesc[] = "# Converted from the color filter:\n#\n"
	"#\t[r,g,b] = [%.10g, %.10g, %.10g]\n"
	"#\tdensity =  %.10g\n#\n";

int usage(void) {
	fprintf(stderr, "Usage: sltconv [-i] input [[-o] output] [-t|T|s|c] [-e efficacy]\n");
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

void describe(FTYPE ftype, FILE *f, saluti *slt, double efficacy) {
	switch (ftype) {
		case matrix:
			fprintf(f, matrixdesc, slt->m3x4.red.r, slt->m3x4.red.g, slt->m3x4.red.b, slt->m3x4.red.o,
				slt->m3x4.green.r, slt->m3x4.green.g, slt->m3x4.green.b, slt->m3x4.green.o,
				slt->m3x4.blue.r, slt->m3x4.blue.g, slt->m3x4.blue.b, slt->m3x4.blue.o
			);
			break;
		case chromat:
			fprintf(f, chrmdesc, slt->chrm.model.r, slt->chrm.model.g, slt->chrm.model.b,
				slt->chrm.chroma.angle, slt->chrm.chroma.magnitude,
				slt->chrm.chroma.saturation,
				slt->chrm.chroma.black, slt->chrm.chroma.white
			);
			break;
		case cflt:
			fprintf(f, cfltdesc, slt->cflt.r, slt->cflt.g, slt->cflt.b, slt->cflt.d);
			break;
		default:
			break;
	}
	if (efficacy != 1.0) fprintf(f, withefficacy, 100.0*efficacy);
}

int main(int argc, char *argv[]) {
	FILE *f;
	KOLIBA_SLUT sLut;
	saluti slt;
	double csum;
	double efficacy = 1.0;
	unsigned char *ptr = (unsigned char *)&sLut;
	char *strptr;
	char *iname = NULL;
	char *oname = NULL;
	FTYPE ftype;
	int is1d;
	unsigned int i;
	char cltt = 0;
	char bin  = 0;

	fprintf(stderr, notice);

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
#ifdef WIN32
			|| (argv[i][0] == '/')
#endif
			) switch (argv[i][1]) {
				case 'e':
					if (argv[i][2] != '\0') strptr = &(argv[i][2]);
					else if ((argv[++i]) != NULL) strptr = argv[i];
					else return usage();
					efficacy = atof(strptr);
					for (;*strptr;strptr++) {
						if (*strptr == '%') efficacy /= 100.0;
					}
					break;
				case 'i':
					if (argv[i][2] != '\0') iname = &(argv[i][2]);
					else if ((argv[++i]) != NULL) iname = argv[i];
					else return (usage());
					break;
				case 'o':
					if (argv[i][2] != '\0') oname = &(argv[i][2]);
					else if ((argv[++i]) != NULL) oname = argv[i];
					else return (usage());
					break;
				case 't':
					cltt = 1;
					bin  = 0;
					break;
				case 'T':
					cltt = -1;
					bin  =  0;
					break;
				case 's':
					cltt = 0;
					bin  = 1;
					break;
				case 'c':
					cltt = 0;
					bin  = 0;
					break;
				default:
					return usage();
					break;
			}
			else if (iname == NULL) iname = argv[i];
			else if (oname == NULL) oname = argv[i];
			else return usage();
	}

	if (iname == NULL)
		return usage();

	f = fopen(iname, "rb");
	if (f == NULL) return nofile(iname);

	if (fread(ptr, 1, SLTCFILEHEADERBYTES, f) < SLTCFILEHEADERBYTES)
		return invalid(f, iname);

	if ( fseek(f, -(SLTCFILEHEADERBYTES), SEEK_CUR) != 0) {
		fclose(f);
		fprintf(stderr, "Could not rewind file '%s'\n", iname);
		return 37;
	}

	ptr[SLTCFILEHEADERBYTES] = '\0';

	if (memcmp(ptr, KOLIBA_sLutHeader, SLTCFILEHEADERBYTES) == 0) {
		if (KOLIBA_ReadSlutFromOpenFile(&sLut, f) == NULL)
			return invalid(f, iname);
		else ftype = slut;
	}
	else if (memcmp(ptr, KOLIBA_m3x4Header, SLTCFILEHEADERBYTES) == 0) {
		if (KOLIBA_ConvertMatrixToSlut(&sLut, KOLIBA_ReadMatrixFromOpenFile(&slt.m3x4, f)) == NULL)
			return invalid(f, iname);
		else ftype = matrix;
	}
	else if (memcmp(ptr, KOLIBA_chrmHeader, SLTCFILEHEADERBYTES) == 0) {
		KOLIBA_MATRIX mat;
		if (KOLIBA_ConvertMatrixToSlut(&sLut, KOLIBA_ChromaticMatrix(&mat, KOLIBA_ReadChromaticMatrixFromOpenFile(&slt.chrm, f))) == NULL)
			return invalid(f, iname);
		else ftype = chromat;
	}
	else if (memcmp(ptr, KOLIBA_cFltHeader, SLTCFILEHEADERBYTES) == 0) {
		if (KOLIBA_ConvertColorFilterToSlut(&sLut, KOLIBA_ReadColorFilterFromOpenFile(&slt.cflt, f)) == NULL)
			return invalid(f, iname);
		else ftype = cflt;
	}
	else if (KOLIBA_ReadSlttFromOpenFile(&sLut, f) != NULL)
		ftype = slut;
	else return invalid(f, iname);

	fclose(f);

	if (efficacy != 1.0) KOLIBA_SlutEfficacy(&sLut, &sLut, efficacy);
	KOLIBA_FixSlut(&sLut);

	if (oname != NULL) {
		f = fopen(oname, "wb");
		if (f == NULL) return nofile(oname);
	}
	else f = stdout;

	if (cltt != '\0') {
		if (i = KOLIBA_WriteSlttToOpenFile(&sLut, f)) {
			if (oname) fclose(f);
			return i;
		}
	}
	else if (bin != '\0') {
		if (i = KOLIBA_WriteSlutToOpenFile(&sLut, f)) {
			if (oname) fclose(f);
			return i;
		}
	}
	else {
		is1d = KOLIBA_SlutIs1D(&sLut);

		fprintf(f, cubehead, iname, is1d ? 1 : 3);
		describe(ftype, f, &slt, efficacy);
		fprintf(f, cubeline, sLut.black.r, sLut.black.g, sLut.black.b);

		if (!is1d) {
			fprintf(f, cubeline, sLut.red.r, sLut.red.g, sLut.red.b);
			fprintf(f, cubeline, sLut.green.r, sLut.green.g, sLut.green.b);
			fprintf(f, cubeline, sLut.yellow.r, sLut.yellow.g, sLut.yellow.b);
			fprintf(f, cubeline, sLut.blue.r, sLut.blue.g, sLut.blue.b);
			fprintf(f, cubeline, sLut.magenta.r, sLut.magenta.g, sLut.magenta.b);
			fprintf(f, cubeline, sLut.cyan.r, sLut.cyan.g, sLut.cyan.b);
		}
		fprintf(f, cubeline, sLut.white.r, sLut.white.g, sLut.white.b);
	}
	if ((cltt <= 0) && (bin == 0)) fprintf(f, "\n## Converted from \"%s\" by sltconv, " version "\n\n", iname);
	if (cltt>0) describe(ftype, f, &slt, efficacy);

	if (f != stdout) fclose(f);

	return 0;
}
