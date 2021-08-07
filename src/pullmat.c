/*
	Copyright 2019-21 G. Adam Stanislav
	All rights reserved.

	Extract a matrix from a simple lookup table.
	Issue a warning if it produces a different
	effect from the one in the sLut.
*/

#define	USECLIB
#include <koliba.h>
#include <stdio.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s input.sLut [output.m3x4]\n\n"
	"If no output is specified, the input will be tested\n"
	"for matrix compatibility.\n\n", path);
	return 1;
}

int notslut(char *path, char *file) {
	fprintf(stderr, "%s: %s is not a valid sLut file\n", path, file);
	return 3;
}

int main(int argc, char *argv[]) {
	KOLIBA_SLUT sLut;
	KOLIBA_MATRIX mat;
	int is;
	double sum;

	if ((argc != 2) && (argc != 3)) return usage(argv[0]);

	if (KOLIBA_ReadSlutFromCompatibleNamedFile( &sLut, argv[1], NULL) == NULL)
 		return notslut(argv[0], argv[1]);

	if ((is = KOLIBA_SlutIsMatrix(&sLut)) == 0)
		fprintf(stderr, "%sThe matrix w%s only be a simplified version of %s.\n", "Warning: " + (argc == 2) * 9, (argc == 3) ? "ill" : "ould", argv[1]);

	if (KOLIBA_ConvertSlutToMatrix(&mat, &sLut) == NULL) {
		fprintf(stderr, "%s: Failed to convert %s to a matrix\n", argv[0], argv[1]);
		return 4;
	}

	if (argc == 3) {
		if (is == 0) {
			// If the original was normalized, try normalizing
			// the matrix. Whether it helps is another matter.
			if ((sLut.black.r == 0.0) && (sLut.white.r == 1.0)) {
				if ((sum = mat.red.r + mat.red.g + mat.red.b) != 0.0) {
					mat.red.r /= sum;
					mat.red.g /= sum;
					mat.red.b /= sum;
				}

				if ((sum = mat.green.r + mat.green.g + mat.green.b) != 0.0) {
					mat.green.r /= sum;
					mat.green.g /= sum;
					mat.green.b /= sum;
				}

				if ((sum = mat.blue.r + mat.blue.g + mat.blue.b) != 0.0) {
					mat.blue.r /= sum;
					mat.blue.g /= sum;
					mat.blue.b /= sum;
				}
			}
		}

		if (KOLIBA_WriteMatrixToNamedFile(&mat, argv[2])) {
			fprintf(stderr, "%s: Could not write file '%s'.\n", argv[0], argv[2]);
			return 3;
		}
	}
	else if (is !=0) fprintf(stderr, "%s may be converted to a matrix\nwith no loss of precision.\n");

	return 0;
}
