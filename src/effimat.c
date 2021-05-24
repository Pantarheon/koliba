/*
	Copyright 2019 G. Adam Stanislav
	All rights reserved.

	Apply efficacy to a matrix.
*/

#include <koliba.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s input.m3x4 output.m3x4 [efficacy]\n\n", path);
	return 1;
}

int notmat(char *path, char *file, FILE *input) {
	fprintf(stderr, "%s: %s is not a valid m3x4 file\n", path, file);
	fclose(input);
	return 3;
}

int main(int argc, char *argv[]) {
	FILE *f;
	KOLIBA_MATRIX2 mat;
	char buffer[SLTCFILEHEADERBYTES];


	if ((argc != 3) && (argc != 4)) return usage(argv[0]);

	if ((f = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		return 2;
	}

	fread(buffer, 1, SLTCFILEHEADERBYTES, f);
	if (memcmp(buffer, KOLIBA_m3x4Header, SLTCFILEHEADERBYTES) != 0)
		return notmat(argv[0], argv[1], f);

	fread(&mat, 1, sizeof(KOLIBA_MATRIX2), f);
	KOLIBA_FixDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
	if (KOLIBA_CheckMat(&mat.mat,mat.checksum) == 0)
		return notmat(argv[0], argv[1], f);

	fclose(f);

	KOLIBA_MatrixEfficacy(&mat.mat, &mat.mat, (argc == 4) ? atof(argv[3]) : 0.5);

	if ((f = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[2]);
		return 4;
	}

	mat.checksum = KOLIBA_CalcSum((double *)&mat.mat, sizeof(KOLIBA_MATRIX)/sizeof(double));
	KOLIBA_NetDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
	fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
	fwrite(&mat, 1, sizeof(KOLIBA_MATRIX2), f);
	fclose(f);

	return 0;
}
