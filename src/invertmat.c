/*
	Copyright 2019 G. Adam Stanislav
	All rights reserved.

	If possible, invert a matrix, or just tell
	us whether it is invertible.
*/

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
	fclose(input);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *input, *output;
	KOLIBA_MATRIX2 mat;
	char buffer[SLTCFILEHEADERBYTES];
	int sum;


	if ((argc != 2) && (argc != 3)) return usage(argv[0]);

	if ((input = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		return 2;
	}

	fread(buffer, 1, SLTCFILEHEADERBYTES, input);
	if (memcmp(buffer, KOLIBA_m3x4Header, SLTCFILEHEADERBYTES) != 0)
		return notmat(argv[0], argv[1], input);

	fread(&mat, 1, sizeof(KOLIBA_MATRIX2), input);
	KOLIBA_FixDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
	if (KOLIBA_CheckMat(&mat.mat,mat.checksum) == 0)
		return notmat(argv[0], argv[1], input);

	fclose(input);

	if (KOLIBA_InvertMatrix(&mat.mat, &mat.mat) == NULL) {
		fprintf(stderr, "%sThe matrix is not invertible.\n", "Error: " + (argc == 2) * 7);
		return (argc == 3) * 4;
	}

	if (argc == 3) {
		if ((output = fopen(argv[2], "wb")) == NULL) {
			fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[2]);
			return 5;
		}

		mat.checksum = KOLIBA_CalcSum((double *)&mat.mat, sizeof(KOLIBA_MATRIX)/sizeof(double));
		KOLIBA_NetDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
		fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, output);
		fwrite(&mat, 1, sizeof(KOLIBA_MATRIX2), output);
		fclose(output);
	}
	else fprintf(stderr, "%s is an invertible matrix.\n", argv[1]);

	return 0;
}
