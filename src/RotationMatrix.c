/*
	Copyright 2019 G. Adam Stanislav
	All rights reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <koliba.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s output.m3x4 [angle [magnitude [saturation [white [black]]]]]\n\n"
	"If not specified, these are the defaults:\n\n"
	"\tangle:      30 degrees\n"
	"\tmagnitude:  1\n"
	"\tsaturation: 1\n"
	"\twhite:      1\n"
	"\tblack:      0\n", path);
	return 1;
}

int main(int argc, char *argv[]) {
	KOLIBA_MATRIX2 mat;
	FILE *f;
	KOLIBA_CHROMAT chromat;

	memcpy(&chromat.model, &KOLIBA_Rec2020, sizeof(KOLIBA_RGB));
	chromat.chroma.angle      =  30.0;
	chromat.chroma.magnitude  =  1.0;
	chromat.chroma.saturation =  1.0;
	chromat.chroma.black      =  0.0;
	chromat.chroma.white      =  1.0;

	if ((argc < 2) || (argc > 7)) return usage(argv[0]);

	if ((f = fopen(argv[1], "wb")) == NULL) {
		fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[1]);
		return 2;
	}

	if (argc > 2) chromat.chroma.angle      = atof(argv[2]);
	if (argc > 3) chromat.chroma.magnitude  = atof(argv[3]);
	if (argc > 4) chromat.chroma.saturation = atof(argv[4]);
	if (argc > 5) chromat.chroma.white      = atof(argv[5]);
	if (argc > 6) chromat.chroma.black      = atof(argv[6]);


	if (KOLIBA_ChromaticMatrix(&mat.mat, &chromat) != NULL) {
		fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
		mat.checksum = KOLIBA_CalcSum((double *)&mat.mat, sizeof(KOLIBA_MATRIX)/sizeof(double));
		KOLIBA_NetDoubles((double *)&mat, sizeof(KOLIBA_MATRIX2)/sizeof(double));
		fwrite(&mat, 1, sizeof(KOLIBA_MATRIX2), f);
		fclose(f);
	}
	return 0;
}
