/*
	Copyright 2019-2021 G. Adam Stanislav
	All rights reserved.

	If possible, invert a matrix, or just tell
	us whether it is invertible.
*/

#define	USECLIB
#include <koliba.h>
#include <stdio.h>
#include <string.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s input.m3x4 [output.m3x4]\n\n"
	"If no output is specified, the input will be tested\n"
	"for matrix invertibility.\n\n", path);
	return 1;
}

int notmat(char *path, char *file, FILE *input) {
	fprintf(stderr, "%s: %s is not a valid m3x4 file\n", path, file);
	KOLIBA_Close(input);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *input, *output;
	KOLIBA_MATRIX mat;
	char buffer[SLTCFILEHEADERBYTES];
	int sum;


	if ((argc != 2) && (argc != 3)) return usage(argv[0]);

	if ((input = KOLIBA_OpenToRead(argv[1])) == NULL) {
		fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		return 2;
	}

	if (KOLIBA_ReadMatrixFromCompatibleOpenFile(&mat, input, NULL) == NULL)
		return notmat(argv[0], argv[1], input);

	KOLIBA_Close(input);

	if (KOLIBA_InvertMatrix(&mat, &mat) == NULL) {
		fprintf(stderr, "%sThe matrix is not invertible.\n", "Error: " + (argc == 2) * 7);
		return (argc == 3) * 4;
	}

	if (argc == 3) {
		if ((output = KOLIBA_OpenToWrite(argv[2])) == NULL) {
			fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[2]);
			return 5;
		}

		KOLIBA_WriteMatrixToOpenFile(&mat, output);
		KOLIBA_Close(output);
	}
	else fprintf(stderr, "%s is an invertible matrix.\n", argv[1]);

	return 0;
}
