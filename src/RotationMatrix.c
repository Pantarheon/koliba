/*
	Copyright 2019-2021 G. Adam Stanislav
	All rights reserved
*/

#define USECLIB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <koliba.h>

int usage(char *path) {
	fprintf(stderr, "Usage:\n\n\t%s output.chrm [angle [magnitude [saturation [white [black]]]]]\n\n"
	"If not specified, these are the defaults:\n\n"
	"\tangle:      30 degrees\n"
	"\tmagnitude:  1\n"
	"\tsaturation: 1\n"
	"\twhite:      1\n"
	"\tblack:      0\n", path);
	return 1;
}

int main(int argc, char *argv[]) {
	FILE *f;
	KOLIBA_CHROMAT chromat;

	memcpy(&chromat.model, &KOLIBA_Rec2020, sizeof(KOLIBA_RGB));
	chromat.chroma.angle      =  30.0;
	chromat.chroma.magnitude  =  1.0;
	chromat.chroma.saturation =  1.0;
	chromat.chroma.black      =  0.0;
	chromat.chroma.white      =  1.0;

	if ((argc < 2) || (argc > 7)) return usage(argv[0]);

	if ((f = KOLIBA_OpenToWrite(argv[1])) == NULL) {
		fprintf(stderr, "%s: Could not create file %s\n", argv[0], argv[1]);
		return 2;
	}

	if (argc > 2) chromat.chroma.angle      = atof(argv[2]);
	if (argc > 3) chromat.chroma.magnitude  = atof(argv[3]);
	if (argc > 4) chromat.chroma.saturation = atof(argv[4]);
	if (argc > 5) chromat.chroma.white      = atof(argv[5]);
	if (argc > 6) chromat.chroma.black      = atof(argv[6]);

	KOLIBA_WriteChromaticMatrixToOpenFile(&chromat, f);
	KOLIBA_Close(f);
	return 0;
}
