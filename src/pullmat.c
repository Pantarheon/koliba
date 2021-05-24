/*
	Copyright 2019 G. Adam Stanislav
	All rights reserved.

	Extract a matrix from a simple lookup table.
	Issue a warning if it produces a different
	effect from the one in the sLut.
*/

#include <koliba.h>
#include <stdio.h>
#include <string.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s input.sLut [output.m3x4]\n\n"
	"If no output is specified, the input will be tested\n"
	"for matrix compatibility.\n\n", path);
	return 1;
}

int notslut(char *path, char *file, FILE *input) {
	fprintf(stderr, "%s: %s is not a valid sLut file\n", path, file);
	fclose(input);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *input, *output;
	KOLIBA_SLUT2 slut;
	KOLIBA_MATRIX2 mat;
	char buffer[SLTCFILEHEADERBYTES];
	int sum, is;

	if ((argc != 2) && (argc != 3)) return usage(argv[0]);

	if ((input = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		return 2;
	}

	fread(buffer, 1, SLTCFILEHEADERBYTES, input);
	if (memcmp(buffer, KOLIBA_sLutHeader, SLTCFILEHEADERBYTES) != 0)
		return notslut(argv[0], argv[1], input);

	fread(&slut, 1, sizeof(KOLIBA_SLUT2), input);
	KOLIBA_FixDoubles((double *)&slut, sizeof(KOLIBA_SLUT2)/sizeof(double));
	if (KOLIBA_CheckSlut(&slut.sLut,slut.checksum) == 0)
		return notslut(argv[0], argv[1], input);

	if ((is = KOLIBA_SlutIsMatrix(&slut.sLut)) == 0)
		fprintf(stderr, "%sThe matrix w%s only be a simplified version of %s.\n", "Warning: " + (argc == 2) * 9, (argc == 3) ? "ill" : "ould", argv[1]);

	fclose(input);

	if (KOLIBA_ConvertSlutToMatrix(&mat.mat, &slut.sLut) == NULL) {
		fprintf(stderr, "%s: Failed to convert %s to a matrix\n", argv[0], argv[1]);
		return 4;
	}

	if (argc == 3) {
		if ((output = fopen(argv[2], "wb")) == NULL) {
			fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[2]);
			return 5;
		}

		if (is == 0) {
			// If the original was normalized, try normalizing
			// the matrix. Whether it helps is another matter.
			if ((slut.sLut.black.r == 0.0) && (slut.sLut.white.r == 1.0)) {
				if ((sum = mat.mat.red.r + mat.mat.red.g + mat.mat.red.b) != 0.0) {
					mat.mat.red.r /= sum;
					mat.mat.red.g /= sum;
					mat.mat.red.b /= sum;
				}

				if ((sum = mat.mat.green.r + mat.mat.green.g + mat.mat.green.b) != 0.0) {
					mat.mat.green.r /= sum;
					mat.mat.green.g /= sum;
					mat.mat.green.b /= sum;
				}

				if ((sum = mat.mat.blue.r + mat.mat.blue.g + mat.mat.blue.b) != 0.0) {
					mat.mat.blue.r /= sum;
					mat.mat.blue.g /= sum;
					mat.mat.blue.b /= sum;
				}
			}
		}

		mat.checksum = KOLIBA_CalcSum((double *)&mat.mat, sizeof(KOLIBA_MATRIX)/sizeof(double));
		KOLIBA_NetDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
		fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, output);
		fwrite(&mat, 1, sizeof(KOLIBA_MATRIX2), output);
		fclose(output);
	}
	else if (is !=0) fprintf(stderr, "%s may be converted to a matrix\nwith no loss of precision.\n");

	return 0;
}
