/*
	ConvertRecs.c

	Copyright 2019 G. Adam Stanislav
	All rights reserved.

	This is a test of the koliba library.
	It creates several matrices to convert
	between Rec. 601, Rec. 709, and Rec. 2020.

	Why would anyone want to convert them
	like that? Well, if you have a digital
	clip (or photo) shot back when Rec. 601
	was the king (actually queen, since Rec.
	stands for Recommendation which comes from
	the Latin recommendatio, a feminine noun),
	but you want to show it on an HDTV or 4k
	using the Rec. 2020 or Rec. 2100 (both
	use the same conversion), using the proper
	matrix lets you accomplish that goal.

	And of course, this code shows how to
	create such matrices in your own programs.
*/

#include <koliba.h>
#include <stdio.h>

int main() {
	KOLIBA_MATRIX2 A;
	FILE *f;
	unsigned retval = 0;

	if (KOLIBA_ConvertRgb601To709(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 601->709 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec601ToRec709.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 601->709 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	if (KOLIBA_ConvertRgb601To2020(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 601->2020 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec601ToRec2020.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 601->2020 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	if (KOLIBA_ConvertRgb709To2020(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 709->2020 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec709ToRec2020.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 709->2020 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	if (KOLIBA_ConvertRgb709To601(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 709->601 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec709ToRec601.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 709->601 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	if (KOLIBA_ConvertRgb2020To601(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 2020->601 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec2020ToRec601.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 2020->601 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	if (KOLIBA_ConvertRgb2020To709(&A.mat) == NULL) {
		fprintf(stderr, "ConvertRecs: Could not produce 2020->709 matrix.\n");
		retval++;
	}

	else {
		if ((f = fopen("ConvertRec2020ToRec709.m3x4", "wb")) == NULL) {
			fprintf(stderr, "ConvertRecs: Could not create 2020->709 file.\n");
			retval++;
		}

		else {
			fwrite(KOLIBA_m3x4Header, 1, SLTCFILEHEADERBYTES, f);
			A.checksum = KOLIBA_CalcSum((double *)&A.mat, sizeof(KOLIBA_MATRIX) / sizeof(double));
			KOLIBA_NetDoubles((double *)&A, sizeof(KOLIBA_MATRIX2) / sizeof(double));
			fwrite(&A, 1, sizeof(KOLIBA_MATRIX2), f);
			fclose(f);
		}
	}

	return retval;
}