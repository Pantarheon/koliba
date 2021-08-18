/*
	Copyright 2018-2021 G. Adam Stanislav
	All rights reserved
*/

#define	USECLIB
#include <koliba.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	KOLIBA_DICHROMA dicr;
	unsigned int channel;
	unsigned int normalize;

	if (argc < 2) {
		fprintf(stderr, "Usage: dichromatic filename [angle [saturation [channel [normalize [rotation [black [white [efficacy]]]]]]]]\n");
		return 1;
	}

	KOLIBA_ResetChromaticMatrix(&(dicr.chr), &KOLIBA_Rec2020);
	dicr.chr.chroma.angle = (argc > 2)? atof(argv[2]) : 0.0;
	if ((channel = (argc > 4) ? atoi(argv[4]) : 0) > 2) {
		fprintf(stderr, "dichromatic error: channel must be between 0 and 2");
		return 2;
	}
	normalize = (argc > 5) ? atoi(argv[5]) : 1;
	dicr.chr.chroma.saturation = (argc > 3) ? atof(argv[3]) : 2.0;
	dicr.rotation  = (argc > 6) ? atof(argv[6]) : 0.0;
	if (argc > 7) dicr.chr.chroma.black = atof(argv[7]);
	if (argc > 8) dicr.chr.chroma.white = atof(argv[8]);
	dicr.efficacy  = (argc > 9) ? atof(argv[9]) : 1.0;
	if (KOLIBA_WriteDichromaticMatrixToNamedFile(&dicr, argv[1], normalize, channel ) != 0) {
		fprintf(stderr, "dichromatic: Cannot create file %s\n", argv[1]);
		return 3;
	}

	return 0;
}
