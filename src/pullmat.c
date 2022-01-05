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
	bool is;
	double sum;

	if ((argc != 2) && (argc != 3)) return usage(argv[0]);

	if (KOLIBA_ReadSlutFromCompatibleNamedFile( &sLut, argv[1], NULL) == NULL)
 		return notslut(argv[0], argv[1]);

	if ((is = KOLIBA_SlutIsMatrix(&sLut)) == false)
		fprintf(stderr, "%sThe matrix w%s only be a simplified version of %s.\n", "Warning: " + (argc == 2) * 9, (argc == 3) ? "ill" : "ould", argv[1]);

	if (KOLIBA_ConvertSlutToMatrix(&mat, &sLut, false) == NULL) {
		fprintf(stderr, "%s: Failed to convert %s to a matrix\n", argv[0], argv[1]);
		return 4;
	}

	if (argc == 3) {
		if (is == 0) {
			// If the original was normalized, try normalizing
			// the matrix. Whether it helps is another matter.
			if ((sLut.Black.r == 0.0) && (sLut.White.r == 1.0)) {
				if ((sum = mat.Red.r + mat.Red.g + mat.Red.b) != 0.0) {
					mat.Red.r /= sum;
					mat.Red.g /= sum;
					mat.Red.b /= sum;
				}

				if ((sum = mat.Green.r + mat.Green.g + mat.Green.b) != 0.0) {
					mat.Green.r /= sum;
					mat.Green.g /= sum;
					mat.Green.b /= sum;
				}

				if ((sum = mat.Blue.r + mat.Blue.g + mat.Blue.b) != 0.0) {
					mat.Blue.r /= sum;
					mat.Blue.g /= sum;
					mat.Blue.b /= sum;
				}
			}
		}

		if (KOLIBA_WriteMatrixToNamedFile(&mat, argv[2])) {
			fprintf(stderr, "%s: Could not write file '%s'.\n", argv[0], argv[2]);
			return 3;
		}
	}
	else if (is !=0) fprintf(stderr, "%s may be converted to a matrix\nwith no loss of precision.\n", argv[1]);

	return 0;
}
