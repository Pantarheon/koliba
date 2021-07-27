#define USECLIB
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <koliba.h>

#define	ver	"v.0.1"

#define	ZFLAG	1
#define	OFLAG	2
#define	HFLAG	4
#define	TFLAG	8
#define	FFLAG	1
#define	XFLAG	2

int usage(int err) {
	fprintf((err) ? stderr:stdout, "Usage: quintluts [options] label\n\n"
		"\t-[12345]\n"
		"\t-[RGBCMY]\n"
		"\t-a file\n"
		"\t-b value\n"
		"\t-e value\n"
		"\t-f[TZOHFX]\n"
		"\t-g value\n"
		"\t-i file\n"
		"\t-l label\n"
		"\t-m\n"
		"\t-r value\n\n"
		"The r, g, b values must be between 0 and 1.\n"
		"The value of e (efficacy) should not be 0.\n\n"
	);
	return err;
}

int main(unsigned int argc, char *argv[]) {
	KOLIBA_PALETTE plt;
	KOLIBA_SLUT sLut, slt, alt;
	KOLIBA_QUINTARYCOLORS col;
	char *str;
	char *label = "LUT";
	char *imp = NULL;
	char *amb = NULL;
	double r = 1.0;
	double g = 0.0;
	double b = 0.0;
	double efficacy = -1.0;
	KOLIBA_Pluts pluts = KOLIBA_PlutRed;
	unsigned int i;
	unsigned int step = 8;
	char zoht = TFLAG;
	char fx   = FFLAG;
	KOLIBA_ftype ft;

	fprintf(stderr, "quintluts " ver "\nCopyright 2021 G. Adam Stanislav\nAll rightsreserved\n\n");

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
#ifdef _WIN32
			|| (argv[i][0] == '/')
#endif
		) switch (argv[i][1]) {
			case 'R':
				pluts = KOLIBA_PlutRed;
				break;
			case 'G':
				pluts = KOLIBA_PlutGreen;
				break;
			case 'B':
				pluts = KOLIBA_PlutBlue;
				break;
			case 'C':
				pluts = KOLIBA_PlutCyan;
				break;
			case 'M':
				pluts = KOLIBA_PlutMagenta;
				break;
			case 'Y':
				pluts = KOLIBA_PlutYellow;
				break;
			case 'r':
				if (argv[i][2] != '\0') str = &(argv[i][2]);
				else if (i < (argc - 1)) str = argv[++i];
				else return usage(1);
				r = atof(str);
				break;
			case 'g':
				if (argv[i][2] != '\0') str = &(argv[i][2]);
				else if (i < (argc - 1)) str = argv[++i];
				else return usage(1);
				g = atof(str);
				break;
			case 'b':
				if (argv[i][2] != '\0') str = &(argv[i][2]);
				else if (i < (argc - 1)) str = argv[++i];
				else return usage(1);
				b = atof(str);
				break;
			case 'e':
				if (argv[i][2] != '\0') str = &(argv[i][2]);
				else if (i < (argc - 1)) str = argv[++i];
				else return usage(1);
				efficacy = -fabs(atof(str));
				break;
			case 'i':
				if (argv[i][2] != '\0') imp = &(argv[i][2]);
				else if (i < (argc - 1)) imp = argv[++i];
				else return usage(1);
				break;
			case 'a':
				if (argv[i][2] != '\0') amb = &(argv[i][2]);
				else if (i < (argc - 1)) amb = argv[++i];
				else return usage(1);
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
				step = KOLIBA_QuintarySteps(argv[i][1] - '0');
				break;
			case 'f':
				zoht = 0; fx = 0;
				for (str = &(argv[i][2]); *str; str++) switch (*str) {
					case 'T':
						zoht |= TFLAG;
						break;
					case 'Z':
						zoht |= ZFLAG;
						break;
					case 'O':
						zoht |= OFLAG;
						break;
					case 'H':
						zoht |= HFLAG;
						break;
					case 'F':
						fx   |= FFLAG;
						break;
					case 'X':
						fx   |= XFLAG;
						break;
					default:
						fprintf(stderr, "quintluts: Ignoring unknown flag '%c'.\n", *str);
						break;
				}
				break;
			case 'l':
				if (argv[i][2] != '\0') label = &(argv[i][2]);
				else if (i < (argc - 1)) label = argv[++i];
				else return usage(1);
				break;
			case 'h':
#ifdef _WIN32
			case '?':
#endif
				return usage (0);
				break;
			default:
				return usage(1);
		}
		else label = argv[i];
	}

	if (imp == NULL) {
		if ((r < 0.0) || (g < 0.0) || (b < 0.0) ||
			(r > 1.0) || (g > 1.0) || (b > 1.0)) return usage(2);

		memcpy(&plt, &KOLIBA_IdentityPalette, sizeof(KOLIBA_PALETTE));
		switch (pluts) {
			default:
				plt.red.r           = r;
				plt.red.g           = g;
				plt.red.b           = b;
				plt.red.efficacy    = efficacy;
				break;
			case KOLIBA_PlutGreen:
				plt.green.r         = r;
				plt.green.g         = g;
				plt.green.b         = b;
				plt.green.efficacy  = efficacy;
				break;
			case KOLIBA_PlutBlue:
				plt.blue.r          = r;
				plt.blue.g          = g;
				plt.blue.b          = b;
				plt.blue.efficacy   = efficacy;
				break;
			case KOLIBA_PlutCyan:
				plt.cyan.r          = r;
				plt.cyan.g          = g;
				plt.cyan.b          = b;
				plt.cyan.efficacy   = efficacy;
				break;
			case KOLIBA_PlutMagenta:
				plt.magenta.r        = r;
				plt.magenta.g        = g;
				plt.magenta.b        = b;
				plt.magenta.efficacy = efficacy;
				break;
			case KOLIBA_PlutYellow:
				plt.yellow.r         = r;
				plt.yellow.g         = g;
				plt.yellow.b         = b;
				plt.yellow.efficacy  = efficacy;
				break;
		}

		KOLIBA_ApplyPaletteRing(&plt, &plt, KOLIBA_PlutRed);
		KOLIBA_ConvertPaletteToSlut(&slt, &plt);
	}
	else {
		if (!KOLIBA_ReadSlutFromCompatibleNamedFile(&slt, imp, &ft)) {
			switch (ft) {
				default:
				case KOLIBA_ftnofile:
					str = "quintluts: Error importing file '%s'.\n";
					break;
				case KOLIBA_ftunknown:
					str = "quintluts: Unsupported file type '%s'.\n";
					break;
			}
			fprintf(stderr, str, imp);
			return 7;
		}
	}

	if (amb != NULL) {
		if (!KOLIBA_ReadSlutFromCompatibleNamedFile(&alt, amb, &ft)) {
			switch (ft) {
				default:
				case KOLIBA_ftnofile:
					str = "quintluts: Error importing file '%s'.\n";
					break;
				case KOLIBA_ftunknown:
					str = "quintluts: Unsupported file type '%s'.\n";
					break;
			}
			fprintf(stderr, str, amb);
			return 13;
		}
	}

	str = malloc(strlen(label)+128);

	for (col = KQC_red; col < KQC_COUNT; col += step) {
		if (amb != NULL) {
			if (((!fx) || (fx & FFLAG)) && (KOLIBA_ApplySphericalEfficaciesF(&sLut, &slt, col, &alt))) {
				sprintf(str, "%s_%s-AF.sLut", label, KOLIBA_QuintaryColorTokens[col]);
				if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
					fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
					free (str);
					return 3;
				}
			}
			if (((!fx) || (fx & XFLAG)) && (KOLIBA_ApplySphericalEfficaciesX(&sLut, &slt, col, &alt))) {
				sprintf(str, "%s_%s-AX.sLut", label, KOLIBA_QuintaryColorTokens[col]);
				if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
					fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
					free (str);
					return 3;
				}
			}
		}
		else {
			if ((!zoht) || (zoht & TFLAG) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesF(&sLut, &slt, col, NULL)))) {
				sprintf(str, "%s_%s-TF.sLut", label, KOLIBA_QuintaryColorTokens[col]);
				if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
					fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
					free (str);
					return 3;
				}
			}
			if ((!zoht) || (zoht & ZFLAG)) {
				if (((!fx) || (fx & FFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesF(&sLut, &slt, col, &KOLIBA_NoFarbaSlut)))) {
					sprintf(str, "%s_%s-ZF.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
				if (((!fx) || (fx & XFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesX(&sLut, &slt, col, &KOLIBA_NoFarbaSlut)))) {
					sprintf(str, "%s_%s-ZX.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
			}
			if ((!zoht) || (zoht & OFLAG)) {
				if (((!fx) || (fx & FFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesF(&sLut, &slt, col, &KOLIBA_IdentitySlut)))) {
					sprintf(str, "%s_%s-OF.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
				if (((!fx) || (fx & XFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesX(&sLut, &slt, col, &KOLIBA_IdentitySlut)))) {
					sprintf(str, "%s_%s-OX.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
			}
			if ((!zoht) || (zoht & HFLAG)) {
				if (((!fx) || (fx & FFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesF(&sLut, &slt, col, &KOLIBA_HomeSlut)))) {
					sprintf(str, "%s_%s-HF.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
				if (((!fx) || (fx & XFLAG)) && (!KOLIBA_IsIdentitySlut(KOLIBA_ApplySphericalEfficaciesX(&sLut, &slt, col, &KOLIBA_HomeSlut)))) {
					sprintf(str, "%s_%s-HX.sLut", label, KOLIBA_QuintaryColorTokens[col]);
					if (KOLIBA_WriteSlutToNamedFile(&sLut, str)) {
						fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
						free (str);
						return 3;
					}
				}
			}
		}
	}
	if (!KOLIBA_IsIdentitySlut(&slt)) {
		sprintf(str, "%s_Base.sLut", label);
		if (KOLIBA_WriteSlutToNamedFile(&slt, str)) {
			fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
			free (str);
			return 4;
		}
	}
	if ((amb != NULL) && (!KOLIBA_IsIdentitySlut(&alt))) {
		sprintf(str, "%s_Ambient.sLut", label);
		if (KOLIBA_WriteSlutToNamedFile(&alt, str)) {
			fprintf(stderr, "quintluts: Unable to create file '%s'. Aborting.\n", str);
			free (str);
			return 4;
		}
	}
	free(str);
	return 0;
}
