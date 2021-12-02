/*
	Copyright 2021 G. Adam Stanislav
	All rights reserved.

	Multiply a matrix by one or more
	other matrices.
*/

#define	USECLIB
#include <koliba.h>
#include <stdio.h>
#include <string.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s output input multiplier [...]\n", path);
	return 1;
}

int notmat(char *path, char *file, FILE *input) {
	fprintf(stderr, "%s: %s is not a valid matrix file\n", path, file);
	KOLIBA_Close(input);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *input, *output;
	KOLIBA_MATRIX imat, omat;
	unsigned int i;


	if (argc < 4) return usage(argv[0]);

	if ((input = KOLIBA_OpenToRead(argv[2])) == NULL) {
		fprintf(stderr, "%s: Could not open input file %s\n", argv[0], argv[2]);
		return 2;
	}

	if (KOLIBA_ReadMatrixFromCompatibleOpenFile(&omat, input, NULL) == NULL)
		return notmat(argv[0], argv[2], input);

	KOLIBA_Close(input);

	for (i = 3; i < argc; i++) {
		if ((input = KOLIBA_OpenToRead(argv[i])) == NULL) {
			fprintf(stderr, "%s: Could not open modifier file %s\n", argv[0], argv[2]);
			return 4;
		}

		if (KOLIBA_ReadMatrixFromCompatibleOpenFile(&imat, input, NULL) == NULL)
			return notmat(argv[0], argv[2], input);
		KOLIBA_Close(input);

		KOLIBA_MultiplyMatrices(&omat, &omat, &imat);
	}

	if ((output = KOLIBA_OpenToWrite(argv[1])) == NULL) {
		fprintf(stderr, "%s: Could not create output file %s\n", argv[0], argv[1]);
			return 5;
	}

	KOLIBA_WriteM34tToOpenFile(&omat, output);

	KOLIBA_Fprintf(output, "\n# The Chain:\n# ==========\n#\n");
	for (i = 2; i < argc; i++) KOLIBA_Fprintf(output, "# %s\n", argv[i]);

	KOLIBA_Close(output);

	return 0;
}
