/*

	koliba.h

	Copyright 2019-2021 G. Adam Stanislav
	All rights reserved

	Redistribution and use in source and binary forms,
	with or without modification, are permitted provided
	that the following conditions are met:

	1. Redistributions of source code must retain the
	above copyright notice, this list of conditions
	and the following disclaimer.

	2. Redistributions in binary form must reproduce the
	above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or
	other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the
	names of its contributors may be used to endorse or
	promote products derived from this software without
	specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef	_KOLIBA_H_
#define	_KOLIBA_H_

#ifdef	SWIG
%rename("%(strip:[KOLIBA_])s") "";
%rename(TwoPi) KOLIBA_2Pi;
%rename(OneEightyDivPi) KOLIBA_180DivPi;
%rename(OneDiv2Pi)	KOLIBA_1Div2Pi;
%rename(OneDiv360)	KOLIBA_1Div360;
%rename(OneDFlutFlags) KOLIBA_1DFlutFlags;
%rename(EightBitBuffer) KOLIBA_8BitBuffer;
%ignore KOLIBA_360;
#endif

#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef	NOKLIBLIB	// Should you need a static library...
# define	KLBHID
# define	KLBDC
#elif defined _WIN32
# define	KLBHID
#ifdef	DLL
# define	KLBDC	__declspec(dllexport)
#else	// !DLL
# define	KLBDC	__declspec(dllimport)
#endif	// DLL
#elif defined __APPLE__ || defined linux || defined __FreeBSD__
#  define KLBDC __attribute__((visibility("default")))
# define	KLBHID __attribute__((visibility("hidden")))
#else	// !_WIN32 !__APPLE__ !linux !__FreeBSD__
# define	KLBDC
# define	KLBHID
#endif	// _WIN32

#ifdef __cplusplus
extern "C" {
#endif


#define	SLTCFILEHEADERBYTES	24
#define	SLTAMINCHARS	(6+17*24)
#define	MATAMINCHARS	(6+17*12)
#define	CHRAMINCHARS	(6+17*8)

typedef	enum {
	KOLIBA_ftnoslut,
	KOLIBA_ftnofile,
	KOLIBA_ftunknown,
	KOLIBA_ftslut,
	KOLIBA_ftmatrix,
	KOLIBA_ftchrm,
	KOLIBA_ftcflt,
	KOLIBA_ftsltt,
	KOLIBA_ftm34t,
	KOLIBA_ftchrt,
	KOLIBA_ftdicr,
	KOLIBA_ftdcrt,
	KOLIBA_ftpalette,
	KOLIBA_ftkptt,
	KOLIBA_ftcftt
} KOLIBA_ftype;

typedef enum {
	KOLIBA_MalletAll = 0,
	KOLIBA_MalletSvit,
	KOLIBA_MalletBlack,
	KOLIBA_MalletWhite,
	KOLIBA_MalletFarba,
	KOLIBA_MalletPrimary,
	KOLIBA_MalletSecondary,
	KOLIBA_MalletWarm,
	KOLIBA_MalletCold,
	KOLIBA_MalletSelene,
	KOLIBA_MalletNyx,
	KOLIBA_MalletOrion,
	KOLIBA_MalletBetelgeuse,
	KOLIBA_MalletRed,
	KOLIBA_MalletGreen,
	KOLIBA_MalletBlue,
	KOLIBA_MalletCyan,
	KOLIBA_MalletMagenta,
	KOLIBA_MalletYellow,
	KOLIBA_FUNDAMENTALMALLETS
} fundmal;

typedef enum {
	KOLIBA_PlutBlack,
	KOLIBA_PlutWhite,
	KOLIBA_PlutRed,
	KOLIBA_PlutGreen,
	KOLIBA_PlutBlue,
	KOLIBA_PlutCyan,
	KOLIBA_PlutMagenta,
	KOLIBA_PlutYellow
} KOLIBA_Pluts;

typedef enum {
	KQC_red,
	KQC_scarlet,
	KQC_vermilion,
	KQC_persimmon,
	KQC_orange,
	KQC_orangepeel,
	KQC_amber,
	KQC_goldenyellow,
	KQC_yellow,
	KQC_lemon,
	KQC_lime,
	KQC_springbud,
	KQC_chartreuse,
	KQC_brightgreen,
	KQC_harlequin,
	KQC_neongreen,
	KQC_green,
	KQC_jade,
	KQC_erin,
	KQC_emerald,
	KQC_springgreen,
	KQC_mint,
	KQC_aquamarine,
	KQC_turquoise,
	KQC_cyan,
	KQC_skyblue,
	KQC_capri,
	KQC_cornflower,
	KQC_azure,
	KQC_cobalt,
	KQC_cerulean,
	KQC_sapphire,
	KQC_blue,
	KQC_iris,
	KQC_indigo,
	KQC_veronica,
	KQC_violet,
	KQC_amethyst,
	KQC_purple,
	KQC_phlox,
	KQC_magenta,
	KQC_fuchsia,
	KQC_cerise,
	KQC_deeppink,
	KQC_rose,
	KQC_raspberry,
	KQC_crimson,
	KQC_amaranth,
	KQC_COUNT
} KOLIBA_QUINTARYCOLORS;

/****************************************************************************/
/*******************                                   **********************/
/******************* T H E  KOLIBA  D A T A  T Y P E S **********************/
/*******************                                   **********************/
/****************************************************************************/

//		Entia non praeter necessitatem multiplicanda sint.

// All look-up tables used by the Koliba library consist of several (mostly 8)
// vertices. The library was originally developed to help create color-grading
// software. Hence, the use of {r, g, b} for the vertex. But the same math can
// be used for interpolation of any 3D data, where an {x, y, z} vertex would
// make more sense. That is why we define two identical structures using two
// different sets of labels. Use whichever is more appropriate to your work,
// and just typecast it to (KOLIBA_VERTEX *), or such, as necessary. So we can
// stay parsimonious with our entia and generous with our titularia.
typedef	struct _KOLIBA_RGB {
	double	r;
	double	g;
	double	b;
} KOLIBA_RGB, KOLIBA_VERTEX;

typedef	struct _KOLIBA_XYZ {
	double	x;
	double	y;
	double	z;
} KOLIBA_XYZ;

// The workhorse of the Koliba library is the FLUT, representing the
// multiplication factors of a simple look-up table.
typedef struct	_KOLIBA_FLUT {
	KOLIBA_VERTEX	black;
	KOLIBA_VERTEX	red;
	KOLIBA_VERTEX	green;
	KOLIBA_VERTEX	blue;
	KOLIBA_VERTEX	yellow;
	KOLIBA_VERTEX	magenta;
	KOLIBA_VERTEX	cyan;
	KOLIBA_VERTEX	white;
} KOLIBA_FLUT;

// The most important support type in the Koliba library is a list of 24
// bit flags, which tell Koliba which factors in the FLUT to use and
// which to ignore. This serves two purposes: (1) to speed up the processing,
// and (2) to allow the user to skip certain values within a FLUT to create
// some special effect. We keep the 24 bit flags in the lower 24 bits of
// a 32-bit integer.
typedef	unsigned int KOLIBA_FLAGS;

// In some cases we need to pass a pointer to an array of FLUTs with their
// corresponding FLAGS. We use the following combined structure for those
// arrays.
typedef struct	_KOLIBA_FFLUT {
	KOLIBA_FLUT		*fLut;
	KOLIBA_FLAGS	 flags;
} KOLIBA_FFLUT;

// While the FLUT is our workhorse, traditionally people have been sharing
// look-up tables that do not contain the multiplication factors, but the
// values of red, green, and blue pixels to be interpolated. We need to be
// able to convert those to our FLUT and FLAGS. For that, we declare a simple
// look-up table, SLUT (pronounced in English the same as "salute").
//
// We might also say the FLUT is the heart of the Koliba library, and SLUT is
// its brain. When developing effects, we always go through various steps of
// producing a SLUT first. It is intuitive, it is logical, it is mathematical.
// It can be applied directly to pixel manipulation (and in the original
// Koliba Palette Mallet OFX plug-in it was), but doing that is slower than
// converting the SLUT into a FLUT (and the corresponding flags), which can,
// and does, work much faster.
typedef struct	_KOLIBA_SLUT {
	KOLIBA_VERTEX	black;
	KOLIBA_VERTEX	blue;
	KOLIBA_VERTEX	green;
	KOLIBA_VERTEX	cyan;
	KOLIBA_VERTEX	red;
	KOLIBA_VERTEX	magenta;
	KOLIBA_VERTEX	yellow;
	KOLIBA_VERTEX	white;
} KOLIBA_SLUT;

// Since, in the .sLut file (defined later), this is followed by a checksum,
// we may want to read it all into this structure:
typedef struct _KOLIBA_SLUT2 {
	KOLIBA_SLUT sLut;
	double checksum;
} KOLIBA_SLUT2;

// The vertices in the SLUT structure are sorted in a mathematically
// meaningful order. We do not use that order when presenting a LUT in user
// interfaces, simply because we tend to think of the palette the vertices
// represent as black, white, red, green, blue, cyan, magenta, yellow.
// So we have a presentation LUT (or PLUT) structure.
//
// In addition to the vertices, this structure also contains a divisor
// (which is expected to be greater than or equal to 1) to accomodate
// systems that present color ranges as {0-255} and such. It also
// contains an efficacy value to make it possible to fade a LUT in
// and out.
typedef struct _KOLIBA_PLUT {
	KOLIBA_VERTEX black;
	KOLIBA_VERTEX white;
	KOLIBA_VERTEX red;
	KOLIBA_VERTEX green;
	KOLIBA_VERTEX blue;
	KOLIBA_VERTEX cyan;
	KOLIBA_VERTEX magenta;
	KOLIBA_VERTEX yellow;
	double	divisor;	// >= 1.0, so we can deal with 8-bit LUTs and such
	double	efficacy;
} KOLIBA_PLUT;

// While we use a certain order of vertices in a SLUT, we may deal with LUT
// data organized differently depending on the source of such data. Assuming
// it still presents the data as a series of red, green, blue (or x, y, z)
// vertices, we can easily use it thanks to our VERTICES pointers.
typedef struct _KOLIBA_VERTICES {
	KOLIBA_VERTEX	*black;
	KOLIBA_VERTEX	*blue;
	KOLIBA_VERTEX	*green;
	KOLIBA_VERTEX	*cyan;
	KOLIBA_VERTEX	*red;
	KOLIBA_VERTEX	*magenta;
	KOLIBA_VERTEX	*yellow;
	KOLIBA_VERTEX	*white;
} KOLIBA_VERTICES;

// For the sake of converting .sLut files to .cube files easily, we can use
// a different pointer structure, which uses the less-intuitive ordering of
// vertices used by those files.
typedef	struct _KOLIBA_CUBE_VERTICES {
	KOLIBA_VERTEX	*black;
	KOLIBA_VERTEX	*red;
	KOLIBA_VERTEX	*green;
	KOLIBA_VERTEX	*yellow;
	KOLIBA_VERTEX	*blue;
	KOLIBA_VERTEX	*magenta;
	KOLIBA_VERTEX	*cyan;
	KOLIBA_VERTEX	*white;
} KOLIBA_CUBE_VERTICES;

// As much as we love to keep our LUTs parsimoniously simple, not everyone
// does (some people even brag about their LUTs being bigger than other
// people's LUTs).
//
// For such larger LUTs, we may use an indexed FLUT. Each index entry consists
// of an x, y, z (or red, green, blue) unsigned 32-bit integer:
typedef struct _KOLIBA_XYZINDEX {
	unsigned int x, y, z;
} KOLIBA_XYZINDEX;
// We then arrange the indices to indicate individual members of a KOLIBA_FLUT
// as doubles distanced from a base.
typedef struct _KOLIBA_FLINDEX {
	KOLIBA_XYZINDEX	black;
	KOLIBA_XYZINDEX	red;
	KOLIBA_XYZINDEX	green;
	KOLIBA_XYZINDEX	blue;
	KOLIBA_XYZINDEX	yellow;
	KOLIBA_XYZINDEX	magenta;
	KOLIBA_XYZINDEX	cyan;
	KOLIBA_XYZINDEX	white;
} KOLIBA_FLINDEX;
// It is possible (indeed, that is the reason to have indices) for several
// indices to indicate the same double. For example, the identity FLUT
// consists of only two values, 0.0 and 1.0, so if we placed a 0.0 at some
// base, and placed a 1.0 right after it, an "Identity FLIndex" would look
// like this:
//
//	KOLIBA_FLINDEX fli = {
//		{0, 0, 0},	// black
//		{1, 0, 0},	// red
//		{0, 1, 0},	// green
//		... etc ...
//	};
//
// And to actually find the proper value of any FLUT member, we would have a
// base like this:
//
//		double base[2] = {0.0, 1.0};
//
// And then to find the value of, say, green.r, which we would call green.x,
// we would take it from base[fli.green.x] (which happens to be 0.0 in this
// case, but of course can be any value in any other case).
//
// This sacrifices some speed for size, so it makes no sense to condense a
// SLUT into an indexed fLut, but it is quite useful to cram a huge LUT (up to
// 256 x 256 x 256 subtables) into a much smaller space.
//
// The index takes some space. The 32-bit unsigned integer is one half the
// size of a double, so we break even if we can reduce the size of a LUT by
// half. In other words, it is useful to crowd the doubles into a smaller
// space when we can shrink the total number of the doubles to much less than
// one half of the original size.
//
// Alternately, if the total number of unique doubles is no more than 256, we
// can compact the indices to one quarter of their size by using 8-bit bytes
// instead of 32-bit unsigned integers.
typedef struct _KOLIBA_XYZBINDEX {
	unsigned char x, y, z;
} KOLIBA_XYZBINDEX;

typedef struct _KOLIBA_FLBINDEX {
	KOLIBA_XYZBINDEX	black;
	KOLIBA_XYZBINDEX	red;
	KOLIBA_XYZBINDEX	green;
	KOLIBA_XYZBINDEX	blue;
	KOLIBA_XYZBINDEX	yellow;
	KOLIBA_XYZBINDEX	magenta;
	KOLIBA_XYZBINDEX	cyan;
	KOLIBA_XYZBINDEX	white;
} KOLIBA_FLBINDEX;
//
// As a compromise, we can use 16-bit unsigned short integers if the total
// number of groups is more than 256 but not more than 65536.
typedef struct _KOLIBA_XYZWINDEX {
	unsigned short x, y, z;
} KOLIBA_XYZWINDEX;

typedef struct _KOLIBA_FLWINDEX {
	KOLIBA_XYZWINDEX	black;
	KOLIBA_XYZWINDEX	red;
	KOLIBA_XYZWINDEX	green;
	KOLIBA_XYZWINDEX	blue;
	KOLIBA_XYZWINDEX	yellow;
	KOLIBA_XYZWINDEX	magenta;
	KOLIBA_XYZWINDEX	cyan;
	KOLIBA_XYZWINDEX	white;
} KOLIBA_FLWINDEX;
//
// Similarly, we may index the FLAGS required for each indexed FLUT. Such an
// index may be an array of unsigned bytes if there are no more than 256 of
// them. Or it may be an unsigned short if there are no more than 65536 of
// them. (By them I mean indices, not FLAGS.) If there are more than 65536,
// there is no point indexing them because we would have to use 32-bit
// indices for 24-bit numbers. A special case may exist when all of the FLAGS
// are the same regardless of which indexed FLUT is used. There is no need for
// an index then, we just need to have a suitable flag to let us know we have
// a "universal" set of FLAGS.
//
// We only need to flag such a special case for the FLAGS, never for the FLUTs
// because if all the base contains is a list of identical values, we would be
// dealing with a simple LUT, and we never index those.
//
// We can summarize all these options in just two dibits, one for the indexed
// FLUTs, the other for the indexed FLAGS. The values within either dibit are
// mutually exclusive, so we may pick one from the one dibit, one from the
// other dibit, and or them together.
//
// So, our options for the fLuts are:
//
//	0 = no indexing used, the entire fLut array is available;
//	1 = a byte index is used;
//	2 = a two-byte (unsigned short or uint16_t) is used;
//	3 = a four-byte (unsigned int or uint32_t) is used.
//
#define	KOLIBA_NOFLINDEX	0
#define	KOLIBA_USEFLBINDEX	1
#define	KOLIBA_USEFLWINDEX	2
#define	KOLIBA_USEFLINDEX	3
//
// And our options for the flags are:
//
//	0 = no indexing, the entire flag array is available;
//	1 = byte index;
//	2 = two-byte index;
//	3 = no index, one 24-bit flag used for everything.
//
#define	KOLIBA_NOFFLINDEX	(0 << 2)
#define	KOLIBA_USEFFLBINDEX	(1 << 2)
#define	KOLIBA_USEFFLWINDEX	(2 << 2)
#define	KOLIBA_USEONEFFLAG	(3 << 2)
// And just for consistency, even if not quite clear
// but perhaps easier to remember:
#define	KOLIBA_USEFFLINDEX	KOLIBA_USEONEFLAG

// To apply a FLUT to a 32-bit floating-point pixel, we need to define a
// suitable 32-bit pixel structure.

typedef	struct _KOLIBA_PIXEL {
	float red;
	float green;
	float blue;
} KOLIBA_PIXEL;

// Strangely, many video editors are still using pixels with 8 bits per
// channel. We might need to deal with them.
typedef	union _KOLIBA_RGBA8PIXEL {
	struct {uint8_t	r, g, b, a;};
	uint32_t px;
} KOLIBA_RGBA8PIXEL;

// And some, e.g. Vegas Pro, prefer BGRA.
typedef union _KOLIBA_BGRA8PIXEL {
	struct {uint8_t	b, g, r, a;};
	uint32_t px;
} KOLIBA_BGRA8PIXEL;

// Additionally, ARGB and ABGR are possible.
typedef union _KOLIBA_ARGB8PIXEL {
	struct {uint8_t	a, r, g, b;};
	uint32_t px;
} KOLIBA_ARGB8PIXEL;
typedef union	_KOLIBA_ABGR8PIXEL {
	struct {uint8_t	a, g, b, r;};
	uint32_t px;
} KOLIBA_ABGR8PIXEL;

// PNG bitmaps may also use unsigned 16-bit integers per channel.
typedef	struct _KOLIBA_RGBA16PIXEL {
	uint16_t	r, g, b, a;
} KOLIBA_RGBA16PIXEL;

// Most professional video editors use 32-bit floats for color grading to
// reduce crushing and clipping.
typedef	struct _KOLIBA_RGBA32PIXEL {
	float	r, g, b, a;
} KOLIBA_RGBA32PIXEL;
typedef struct _KOLIBA_BGRA32PIXEL {
	float	b, g, r, a;
} KOLIBA_BGRA32PIXEL;
typedef struct _KOLIBA_ARGB32PIXEL {
	float	a, r, g, b;
} KOLIBA_ARGB32PIXEL;
typedef struct	_KOLIBA_ABGR32PIXEL {
	float	a, g, b, r;
} KOLIBA_ABGR32PIXEL;

// A matrix is a common tool used in color grading. Strictly mathematically
// speaking, we should work with 4x4 square matrices. In reality, however, the
// last row of any of our matrices happens to always be {0, 0, 0, 1}. Always.
// We, therefore, only need to keep track of the first three rows.
typedef struct	_KOLIBA_ROW {
	double	r;	// = red
	double	g;	// = green
	double	b;	// = blue
	double	o;	// = offset
} KOLIBA_ROW;

typedef struct _KOLIBA_MATRIX {
	KOLIBA_ROW	red;
	KOLIBA_ROW	green;
	KOLIBA_ROW	blue;
} KOLIBA_MATRIX;
// When stored in a file, we use this:
typedef struct _KOLIBA_MATRIX2 {
	KOLIBA_MATRIX mat;
	double checksum;
} KOLIBA_MATRIX2;

// A "channel blend" is a way to present a matrix to the end user, while
// offering the option to normalize any or all rows, as well as have an
// efficacy to bring it closer to or further away from an identity matrix
// (after applying any normalizations that may be needed).
typedef struct _KOLIBA_CHANNELBLEND {
	KOLIBA_MATRIX mat;
	double efficacy;
	unsigned char nr;		// normalize red row
	unsigned char ng;		// normalize green row
	unsigned char nb;		// normalize blue row
	unsigned char na;		// normalize all rows, overruling nr, ng, nb
} KOLIBA_CHANNELBLEND;

// A "chroma matrix" is the base type of several other "matrix" types that
// define changes to apply to a pixel in the YCbCr color space. That is to
// say, it is not a matrix per se, but it can be unequivocally converted into
// our matrix type, providing we use an RGB model in the conversion.
typedef	struct _KOLIBA_CHROMA {
	double	angle;	// in degrees
	double	magnitude;	// angle efficacy
	double	saturation;
	double	black;
	double	white;
} KOLIBA_CHROMA;

// A "chromatic matrix" combines the chroma matrix with the RGB model to use
// for the conversion from the RGB space to the YCbCr space and then back. It
// also is a base type of several other types.
//
// We refer to it as a chromatic matrix because while it is based on the YCbCr
// idea of luma and chroma, ultimately it is easy to convert to an RGBO matrix
// used in Koliba.
//
// In our own terminology, angle, magnitude and saturation affect the farba;
// black and white affect the svit; and the model affects both, albeit
// indirectly as it normally does not change.
typedef	struct _KOLIBA_CHROMAT {
	KOLIBA_RGB model;
	KOLIBA_CHROMA chroma;
} KOLIBA_CHROMAT;

typedef struct _KOLIBA_CHROMAT2 {
	KOLIBA_CHROMAT chromat;
	double checksum;
} KOLIBA_CHROMAT2;

typedef struct _KOLIBA_DICHROMA {
	KOLIBA_CHROMAT	chr;
	double	rotation;
	double	efficacy;
} KOLIBA_DICHROMA;

typedef struct _KOLIBA_DICHROMA2 {
	KOLIBA_DICHROMA	dichroma;
	double	checksum;
} KOLIBA_DICHROMA2;

// The KOLIBA_DICHROMA structure is really just a special case of the
// KOLIBA_ANACHROMA structure.
typedef	struct _KOLIBA_ANACHROMA {
	KOLIBA_RGB	model;
	KOLIBA_CHROMA	chr[2];
	double	rotation;
	double	efficacy;
} KOLIBA_ANACHROMA;

// And the KOLIBA_ANACHROMA structure is really just a special case of the
// KOLIBA_DIACHROMA structure.
typedef	struct _KOLIBA_DIACHROMA {
	KOLIBA_RGB	model;
	KOLIBA_CHROMA	chr[3];
	double	rotation;
	double	efficacy;
} KOLIBA_DIACHROMA;

// A color filter (CFLT) allows us to emulate a color filter attached to the
// front of a camera objective (lens assembly). The "d" member defines the
// density of the filter.
typedef struct _KOLIBA_CFLT {
	double	r;
	double	g;
	double	b;
	double	d;
} KOLIBA_CFLT;

typedef struct _KOLIBA_CFLT2 {
	KOLIBA_CFLT cFlt;
	double checksum;
} KOLIBA_CFLT2;

// Sometimes a simple LUT (SLUT) does not seem enough to represent the desired
// color change, and we might be tempted to create a large LUT in its stead.
//
// In many such cases it is simpler to create a suitable mask and combine the
// result of applying a SLUT with the original image. This mask may need to be
// based either on the original (usually) or on the modified image (rarely).
//
// And quite often such a mask needs to be based on the luminance of each pixel
// (whether increasing or decreasing luminance) or on either the chrominance
// (usually) or the saturation (rarely) of the pixel (never both, though, since
// saturation derives from chrominance). And sometimes even a combination of
// luminance with either the chrominance or the saturation.
//
// Now, from now on, instead of saying "chrominance or saturation" we will use
// "saturance" (a portmanteau word of saturation and chrominance).
//
// Surprisingly, we could not find a way of combining the original and the
// changed image based on these criteria, at least not in the several video
// editing programs we use. So, we came up with Lumidux, which does just that.
//
// The Lumidux structure contains the following data.
//
//		A byte, set to either 0, 1, or -1 indicating whether the luminance
//		is ascending (0), descending (1), or off (-1).
//
//		A byte, set to either 0 or 1, indicating whether to base luma on the
//		background (0) or foreground (1) pixel. When used with an effect, the
//		original pixel is the background, the processed pixel is the
//		foreground.
//
//		A byte set to values betwen 0 and 6, indicating luminous speed.
//
//		A byte, set to 1 if the mask is to be shown, 0 otherwise.
//
//		A byte set to 0, 1, or -1 indicating whether the saturance is
//		ascending (0), descending (1), or off (-1).
//
//		A byte set to 0 or 1, indicating whether to base saturance on the
//		background or foreground pixel.
//
//		A byte set to values between 0 and 6, indicating saturous speed.
//
//		A byte set to 0 to indicate the use of saturation, or 1 for
//		chrominance (collectively referred to by the saturance portmanteau).
//
//		A 64-bit floating point (double) with a low luminance threshold.
//
//		A 64-bit double with a high luminance threshold.
//
//		A 64-bit double with a low saturance threshold.
//
//		A 64-bit double with a high saturance threshold.
//
// When used the way mentioned above (before and after modification of an
// image), the background refers to the original image, foreground to the same
// image after it has been modified by applying a LUT.
//
// Naturally, you can use Lumidux to combine any two images. That said, we
// suggest that whenever used to combine an original image with a modified
// version thereof, you may want to place the original underneath the modified
// version, i.e., use the background for the original and foreground for the
// newly produced mutation. That way you will be using it consistently every
// time, and dish out no surprises.
typedef struct _KOLIBA_LDX {
	signed char ldirection;		// 0, 1, -1
	unsigned char lbase;		// 0, 1
	unsigned char lspeed;		// 0-6
	unsigned char mask;			// 0, 1
	signed char sdirection;		// 0, 1, -1
	unsigned char sbase;		// 0, 1
	unsigned char sspeed;		// 0-6
	unsigned char schroma;		// 0, 1
	double llow;
	double lhigh;
	double slow;
	double shigh;
} KOLIBA_LDX;

// A Koliba Palette is a structure used in the Koliba Palette Mallet OFX
// plug-in, and now here.
//
// It consists of eight "pigments", black, white, red, green, blue, cyan,
// magenta, and yellow. It was conceptually inspired by the automaton in the
// movie Hugo, but in our case the automaton does not draw a picture with a
// pen, it paints an image with a palette of eight pigments (like those used
// in an oil painting). If we change the color of any of the pigments, the
// automaton will not know. It will just paint with the new palette. The
// painting would, on one hand, be the same as the one painted with the
// original palette, yet, on the other hand, it would have a different mood.
//
// This is precisely what a simple LUT (SLUT) does. But it is more intuitive
// to do so with the palette.
//
// Because the palette was created for an OFX plug-in, and the OFX standard
// only allows an RGB color to have values from 0 to 1, while our SLUT does
// not have such limitations, we have added an efficacy field to our pigment
// definition. It allows us to stay within those limitations but convert the
// palette to a SLUT without such limitations. But for portability we impose
// that limit to a pigment and reject it as invalid if any of its RGB channels
// is outside the [0..1] range.
//
// So, we start by defining a pigment, which is a double RGB color with an
// efficacy added.
typedef struct _KOLIBA_PIGMENT {
	double	r;			// [0..1] only.
	double	g;			// [0..1] only.
	double	b;			// [0..1] only.
	double	efficacy;	// Any real value, though keep it realistic.
} KOLIBA_PIGMENT;

// The palette consists of eight pigments, an overall efficacy, but also with
// an erythropy flag. What is erythropy you ask?
//
// Strictly speaking the word should be erythrotropy, but I took the liberty
// to shorten it to erythropy. It tells us to take the red pigment (including
// its efficacy) and ignore all the other pigments in the palette and rotate
// them the same way red is rotated.
//
// Well, actually not all of them. For the purposes of the Koliba color model,
// we refer to the black and white pigments as the svit and to the rest of
// them as the farba. Erythropy only affects the farba (and only if the
// erythropy flag is not 0).
//
// So here is our palette definition.
typedef struct _KOLIBA_PALETTE {
	KOLIBA_PIGMENT	black;
	KOLIBA_PIGMENT	white;
	KOLIBA_PIGMENT	red;
	KOLIBA_PIGMENT	green;
	KOLIBA_PIGMENT	blue;
	KOLIBA_PIGMENT	cyan;
	KOLIBA_PIGMENT	magenta;
	KOLIBA_PIGMENT	yellow;
	double	efficacy;
	unsigned char	erythropy;
} KOLIBA_PALETTE;
// For the use a Koliba palette (.kPlt) file, we use a slightly modified
// structure (this is explained further later in the section describing
// various file formats).
typedef struct _KOLIBA_PALETTE2 {
	KOLIBA_PIGMENT	black;
	KOLIBA_PIGMENT	white;
	KOLIBA_PIGMENT	red;
	KOLIBA_PIGMENT	green;
	KOLIBA_PIGMENT	blue;
	KOLIBA_PIGMENT	cyan;
	KOLIBA_PIGMENT	magenta;
	KOLIBA_PIGMENT	yellow;
	double	efficacy;
	double	checksum;
} KOLIBA_PALETTE2;

// A way to manipulate a SLUT is the palette mallet. The mallet can manipulate
// all vertices at the same time, or just one of them, or a group of them. We
// use several flags to determine which vertices a mallet applies to. And
// since there are only eight vertices in a SLUT, all of these can fit in an
// eight-bit byte, or a C unsigned char.
#define	KOLIBA_SLUTBLACK		0x01
#define	KOLIBA_SLUTBLUE			0x02
#define	KOLIBA_SLUTGREEN		0x04
#define	KOLIBA_SLUTCYAN			0x08
#define	KOLIBA_SLUTRED			0x10
#define	KOLIBA_SLUTMAGENTA		0x20
#define	KOLIBA_SLUTYELLOW		0x40
#define	KOLIBA_SLUTWHITE		0x80
#define	KOLIBA_SLUTALL			0xFF
#define	KOLIBA_SLUTREDS			(KOLIBA_SLUTRED | KOLIBA_SLUTMAGENTA | KOLIBA_SLUTYELLOW)
#define	KOLIBA_SLUTGREENS		(KOLIBA_SLUTGREEN | KOLIBA_SLUTCYAN | KOLIBA_SLUTYELLOW)
#define	KOLIBA_SLUTBLUES		(KOLIBA_SLUTBLUE | KOLIBA_SLUTCYAN | KOLIBA_SLUTMAGENTA)
#define	KOLIBA_SLUTCYANS		(KOLIBA_SLUTCYAN | KOLIBA_SLUTGREEN | KOLIBA_SLUTBLUE)
#define	KOLIBA_SLUTMAGENTAS		(KOLIBA_SLUTMAGENTA | KOLIBA_SLUTRED | KOLIBA_SLUTBLUE)
#define KOLIBA_SLUTYELLOWS		(KOLIBA_SLUTYELLOW | KOLIBA_SLUTRED | KOLIBA_SLUTGREEN)
#define	KOLIBA_SLUTSVIT			(KOLIBA_SLUTBLACK | KOLIBA_SLUTWHITE)
#define	KOLIBA_SLUTPRIMARY		(KOLIBA_SLUTRED | KOLIBA_SLUTGREEN | KOLIBA_SLUTBLUE)
#define	KOLIBA_SLUTSECONDARY	(KOLIBA_SLUTCYAN | KOLIBA_SLUTMAGENTA | KOLIBA_SLUTYELLOW)
#define	KOLIBA_SLUTFARBA		(KOLIBA_SLUTPRIMARY | KOLIBA_SLUTSECONDARY)
#define	KOLIBA_SLUTCOLD			(KOLIBA_SLUTCYANS)
#define	KOLIBA_SLUTWARM			(KOLIBA_SLUTREDS)
#define	KOLIBA_SLUTSELENE		(KOLIBA_SLUTYELLOWS)
#define	KOLIBA_SLUTNYX			(KOLIBA_SLUTBLUES)
#define	KOLIBA_SLUTORION		(KOLIBA_SLUTGREENS)
#define	KOLIBA_SLUTBETELGEUSE	(KOLIBA_SLUTMAGENTAS)
//
// The above flags are used in the palette mallet, defined by the following
// structure (which is adjusted for a better packing order, which is different
// from its pecking order).
typedef struct _KOLIBA_MALLET {	// the squiggles show KOLIBA_IdentityMallet
	KOLIBA_RGB	center;		// the colors of the contrast center	{0.5, 0.5, 0.5}
	double	adjustment;		// how much to adjust the contrast	{0.0}
	double	natcon;			// how much to adjust the natural contrast	{0.0}
	double	lift;			// how much closer to get to white	{0.0}
	double	gain;			// how much to move away from black	{1.0}
	double	offset;			// how much to add to all channels	{0.0}
	double	efficacy;		// the efficacy of everything but saturation	{1.0}
	double	saturation;		// how much to increase/decrease saturation	{1.0}
	unsigned char invert;	// a boolean to tell whether to invert	{0}
	unsigned char flags;	// which vertices to apply the mallet to	{0}
} KOLIBA_MALLET;
// Unlike the packing order, the pecking order determines what to do first
// (since some of the options sort of contradict each other).
//
// Flags have the highest pecking order since they decide which SLUT vertices
// to affect.
//
// Invert (produce a negative) is applied to those vertices first.
//
// Lift is next, except if the flags are *equal* to KOLIBA_SLUTWHITE, in which
// case gain takes its place.
//
// Contrast adjustment is next. It does its job by moving the vertices closer
// to or farther from the contrast center.
//
// Natural contrast is next. It moves vertices closer to or farther away from
// their own natural contrast centers (each vertex has its own immutable
// natural center).
//
// This is followed by applying gain, except for KOLIBA_SLUTWHITE, which gets
// its lift.
//
// Offset is added next.
//
// All of the above is adjusted according to the requested efficacy.
//
// Finally, saturation is adjusted.
//
// Despite calling it the pecking order, with the exception of the flags, the
// things that happen later may override the effect of things that happen
// earlier, so we might say (except for the flags, again), he who pecks last
// pecks best...
//
// Just kidding, of course, as they all affect the result. They just need to
// be evaluated in a consistent order because otherwise we would be getting
// inconsistent results!

// The concept of efficacy appears all over the Koliba library. The efficacy
// is usually a single number used to interpolate a LUT with whatever default
// exists for that type of LUT. When the number is lesser than 0 or greater
// than 1, it results in an extrapolation instead of interpolation. It is
// surprising how often video effects allow interpolation but not
// extrapolation, even though that extends the usefulness of most effects. Of
// course, it may occasionally result in producing garbage, but surely we
// trust our users to decide on their own what is useful and what is grabage
// (especially since they may be making a video on garbage).
//
// Another possible interpolation is applying a separate efficacy to each
// vertex of a LUT for maximum flexibility. To make it easy, we define a "LUT"
// of efficacies.
typedef	struct _KOLIBA_EFFILUT {
	double	black;
	double	blue;
	double	green;
	double	cyan;
	double	red;
	double	magenta;
	double	yellow;
	double	white;
} KOLIBA_EFFILUT;

// In addition to different data types, some Koliba routines accept function
// pointers among their arguments. These need to point at specific function
// types, as defined here.
typedef KOLIBA_XYZ * (*KOLIBA_INDEXEDXYZ)(KOLIBA_XYZ *xout, const KOLIBA_XYZ * const xin, const double * const base, const KOLIBA_FLAGS * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex);
typedef KOLIBA_PIXEL * (*KOLIBAXYZTOPIXEL)(KOLIBA_PIXEL *pixelout, const KOLIBA_XYZ * const xyzin);
typedef KOLIBA_XYZ *(*KOLIBAPIXELTOXYZ)(KOLIBA_XYZ *xyzout, const KOLIBA_PIXEL * const pixelin);
typedef KOLIBA_XYZ * (*KOLIBA_EXTERNAL)(KOLIBA_XYZ *xyz, const void * const params);
typedef KOLIBA_RGB * (*KOLIBA_MAKEVERTEX)(KOLIBA_RGB * rgbout, const KOLIBA_RGB * const rgbin, const void * const params);
typedef	double (*KOLIBA_DBLCONV)(double);


/****************************************************************************/
/*********************                                ***********************/
/********************* T H E  KOLIBA  D E F A U L T S ***********************/
/*********************                                ***********************/
/****************************************************************************/

// Many Koliba data types can (and do) have default values. Such defaults are
// used to set a starting value in an effect or to interpolate and extrapolate
// various data items with/from their defaults (or in some cases, such as
// saturation interpolate the default with the change). The default are often
// called identities (for example, a default matrix is the identity matrix).
// The term identity in these cases comes from mathematics.
//
// All these defaults are included in the Koliba library.
KLBDC extern const KOLIBA_FLUT KOLIBA_IdentityFlut;
KLBDC extern const KOLIBA_SLUT KOLIBA_IdentitySlut;
KLBDC extern const KOLIBA_PLUT KOLIBA_IdentityPlut;
KLBDC extern const KOLIBA_CHANNELBLEND KOLIBA_IdentityChannelBlend;
#define KOLIBA_IdentityMatrix	(KOLIBA_IdentityChannelBlend.mat)
KLBDC extern const KOLIBA_CHROMA KOLIBA_IdentityChroma;
KLBDC extern const KOLIBA_PALETTE KOLIBA_IdentityPalette;
#ifndef	SWIG
KLBHID extern const KOLIBA_MALLET KOLIBA_IdentityMallet;
#endif

// Not exactly a default, but useful, a sLut with all zeros
// except in the white vertex.
KLBDC extern const KOLIBA_SLUT KOLIBA_NoFarbaSlut;

// And one with all channels of primary farba at 0, and of
// secondary farba at 1, which are their "home" values.
KLBDC extern const KOLIBA_SLUT KOLIBA_HomeSlut;




/****************************************************************************/
/**************                                              ****************/
/************** T H E  KOLIBA  D A T A ( C O N S T A N T S ) ****************/
/**************                                              ****************/
/****************************************************************************/

// Here are some useful constants. They are as precise as the 64-bit floating
// point format allows.
KLBDC extern const double KOLIBA_Pi;
KLBDC extern const double KOLIBA_2Pi;
KLBDC extern const double KOLIBA_PiDiv2;
KLBDC extern const double KOLIBA_PiDiv180;
KLBDC extern const double KOLIBA_180DivPi;
KLBDC extern const double KOLIBA_1Div2Pi;
KLBDC extern const double KOLIBA_1Div360;
KLBDC extern const double KOLIBA_360;

// And some macros/inlines for their use.
#ifdef	NOKLINLIN
#define	KOLIBA_ConvertDegreesToRadians(deg)	((double)(deg)*KOLIBA_PiDiv180)
#define	KOLIBA_ConvertRadiansToDegrees(rad)	((double)(rad)*KOLIBA_180DivPi)
#define	KOLIBA_ConvertTurnsToRadians(t)	((double)(t)*KOLIBA_2Pi)
#define	KOLIBA_ConvertRadiansToTurns(rad)	((double)(rad)*KOLIBA_1Div2Pi)
#define	KOLIBA_ConvertDegreesToTurns(deg)	((double)(deg)*KOLIBA_1Div360)
#define	KOLIBA_ConvertTurnsToDegrees(t)	((double)(t)*KOLIBA_360)
#else
inline double KOLIBA_ConvertDegreesToRadians(double deg) {
	return deg * KOLIBA_PiDiv180;
}
inline double KOLIBA_ConvertRadiansToDegrees(double rad) {
	return rad * KOLIBA_180DivPi;
}
inline double KOLIBA_ConvertTurnsToRadians(double t) {
	return t * KOLIBA_2Pi;
}
inline double KOLIBA_ConvertRadiansToTurns(double rad) {
	return rad * KOLIBA_1Div2Pi;
}
inline double KOLIBA_ConvertDegreesToTurns(double deg) {
	return deg * KOLIBA_1Div360;
}
inline double KOLIBA_ConvertTurnsToDegrees(double t) {
	return t * KOLIBA_360;
}
#endif

// These two matrices make it possible to edit RGB images as if they were
// YCbCr images.
//
// To do so, you could follow these steps:
//
//	* Call KOLIBA_RgbToYCC;
//	* Apply KOLIBA_YCbCrShift (modifier) to the result of that call
//   (modificand);
//	* Apply some YCbCr effect (modifier, if matrix);
//	* Apply KOLIBA_YCbCrUnshift (modifier) to the result (modificand);
//	* Call KOLIBA_YccToRGB (modifier).
//
// This is necessary because the range of two channels of YCbCr images is
// {-0.5,0.5} rather than the {0,1} range in all channels of RGB images.
//
// Remember, to modify a matrix A by a matrix B,
// A has to be the modificand and B the modifier
// in the call to KOLIBA_MultiplyMatrices().
KLBDC extern const KOLIBA_MATRIX KOLIBA_YCbCrShift;
KLBDC extern const KOLIBA_MATRIX KOLIBA_YCbCrUnshift;

// Extract a channel, i.e., copy it to all channels
// so we get a grayscale vision of a single channel.
KLBDC extern const KOLIBA_MATRIX KOLIBA_Redx;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Greenx;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Bluex;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Cyanx;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Magentax;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Yellowx;

// Separate a channel from the rest of the channels,
// so we only see that channel in its own color.
KLBDC extern const KOLIBA_MATRIX KOLIBA_Reds;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Greens;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Blues;

// Delete a channel.
KLBDC extern const KOLIBA_MATRIX KOLIBA_Nored;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Nogreen;
KLBDC extern const KOLIBA_MATRIX KOLIBA_Noblue;

// This unsigned char array allows you to convert the enum fundmal type to the
// KOLIBA_SLUT* flags, e.g., KOLIBA_FundamentalMalletFlags[KOLIBA_MalletSvit]
// = KOLIBA_SLUTSVIT. Originally, Koliba library was going to use fundmal in
// various functions, but then we decided it was more flexible to use flags
// that can be combined with the | operator, so if you wanted to apply some
// effect to all the cold farba plus red, KOLIBA_FundamentalMalletFlags
// [KOLIBA_MalletCold] | KOLIBA_FundamentalMalletFlags[KOLIBA_MalletRed] would
// allow you to do it all together.

KLBDC extern const unsigned char KOLIBA_FundamentalMalletFlags[KOLIBA_FUNDAMENTALMALLETS];

// Some lookup tables to speed things up.

KLBDC extern const double KOLIBA_SrgbByteToLinear[256];
KLBDC extern const double KOLIBA_ByteDiv255[256];
KLBDC extern const unsigned char KOLIBA_LinearByteToSrgb[256];

// Quick lookup tables.
//
// Unlike with the simple 8 bpc table, we need two tables
// (one table would certainly work but would be huge),
// one for the high byte, one for the low byte.
//
// This works because (a+b)/c = a/c + b/c, or in our case,
// (HighByte*256+LowByte)/65535 =
// HighByte*256/65535 + LowByte/65535.
// So we pass the high byte as index to HighWordDiv65535,
// the low byte to LowWordDiv65535, and add up the results.
// This is faster than dividing by 65535.0.
//
// We, however, do not have such a pair of tables for sRGB
// because of its non-linearity which would require a huge
// table.
KLBDC extern const uint64_t KOLIBA_HighWordDiv65535[256];
KLBDC extern const uint64_t KOLIBA_LowWordDiv65535[256];

// Some help to produce primary through quintary color LUTs
/*
	These two are deprecated and will be removed in v.1.0.
	Use KOLIBA_SphericalEffilut() instead!
*/
KLBDC extern const KOLIBA_EFFILUT KOLIBA_QuintaryColorsF[];
KLBDC extern const KOLIBA_EFFILUT KOLIBA_QuintaryColorsX[];

/*
	These two replace them internally, but are not exported.
*/
#ifndef	SWIG
KLBHID extern const KOLIBA_EFFILUT KOLIBA_TriFarbaF[3];
KLBHID extern const KOLIBA_EFFILUT KOLIBA_TriFarbaX[3];
#endif

KLBDC extern const char * const KOLIBA_QuintaryColorTokens[KQC_COUNT];
KLBDC extern const char * const KOLIBA_QuintaryColorNames[KQC_COUNT];
// But to allow us to extend it beyond quintary if we so
// choose in the future, this function will give us the
// number of steps to skip in the above tables to find
// just the primary colors, or just the primary and secondary
// colors, etc.
//
// The "ary" argument should be 1 for primary, 2 for secondary,
// etc up to 5 for quintary. If it is larger, it will be trimmed to 5,
// and if it is 0, it will be made 1.
KLBDC unsigned int KOLIBA_QuintarySteps(unsigned int ary);
KLBDC extern const KOLIBA_QUINTARYCOLORS KOLIBA_QuintaryColorCount;


// A pass-through buffer of unmodified 8-bit values.

KLBDC extern const uint8_t KOLIBA_8BitBuffer[256];




/****************************************************************************/
/***************                                            *****************/
/*************** T H E  KOLIBA  I N T E R P O L A T I O N S *****************/
/***************                                            *****************/
/****************************************************************************/

// Interpolation is at the heart of LUT processing. But we also interpolate
// different LUT and other data with each other and with their identity
// (default) values.

KLBDC double * KOLIBA_Interpolate(
	double * output,
	const double * const input,
	double rate,
	const double * const modifier,
	unsigned int count
);

KLBDC float * KOLIBA_SingleInterpolate(
	float * output,
	const float * const input,
	float rate,
	const float * const modifier,
	unsigned int count
);

KLBDC KOLIBA_MATRIX * KOLIBA_InterpolateMatrices(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const input,
	double rate,
	const KOLIBA_MATRIX * const modifier
);

KLBDC KOLIBA_MATRIX * KOLIBA_MatrixEfficacy(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const input,
	double efficacy
);

KLBDC KOLIBA_SLUT * KOLIBA_InterpolateSluts(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input,
	double rate,
	const KOLIBA_SLUT * const modifier
);

KLBDC KOLIBA_SLUT * KOLIBA_SlutEfficacy(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input,
	double efficacy
);

KLBDC KOLIBA_FLUT * KOLIBA_InterpolateFluts(
	KOLIBA_FLUT * output,
	const KOLIBA_FLUT * const input,
	double rate,
	const KOLIBA_FLUT * const modifier
);

KLBDC KOLIBA_FLUT * KOLIBA_FlutEfficacy(
	KOLIBA_FLUT * output,
	const KOLIBA_FLUT * const input,
	double efficacy
);

// A KOLIBA_FundamentalMalletFlags-based interpolation works on just the
// vertices specified by the flags.
//
// Conditionally interpolate vertices of a sLut. This does NOT change those
// vertices in the output not specified in the flags byte.
KLBDC KOLIBA_SLUT * KOLIBA_InterpolateSlutVertices(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input1,
	const KOLIBA_SLUT * const input2,
	double factor,
	unsigned char flags
);

// A more refined way of interpolation is with KOLIBA_EFFILUT, which allows
// you to use a different interpolation for every vertex in a SLUT.

// Initialize all of an EFFILUT to a value.

KLBDC KOLIBA_EFFILUT * KOLIBA_SetEfficacies(
	KOLIBA_EFFILUT * oLut,
	double d
);

// Initialize CMYK of an EFFILUT to one value, the rest to another.

KLBDC KOLIBA_EFFILUT * KOLIBA_SetDualEfficacies(
	KOLIBA_EFFILUT * eLut,
	double cmyk,
	double rgbw
);

// Apply the EFFILUT.

KLBDC KOLIBA_SLUT * KOLIBA_ApplyEfficacies(
	KOLIBA_SLUT *output,
	const KOLIBA_SLUT * const input,
	const KOLIBA_EFFILUT * const efficacies,
	const KOLIBA_SLUT * const modifier
);

// Copy a negative version of an EFFILUT to another (or the same) EFFILUT.

KLBDC KOLIBA_EFFILUT * KOLIBA_InvertEfficacies(
	KOLIBA_EFFILUT * oLut,
	const KOLIBA_EFFILUT * const iLut
);

// An example use would be to apply a different natural contrast to each
// vertex.

KLBDC KOLIBA_SLUT * KOLIBA_ApplyNaturalContrasts(
	KOLIBA_SLUT * sLut,
	const KOLIBA_EFFILUT * const eLut
);

// We can interpolate SLUTs based on Quintary colors:
KLBDC KOLIBA_EFFILUT *KOLIBA_SphericalEffilut(KOLIBA_EFFILUT *effi, double angle, double fx);
KLBDC KOLIBA_SLUT *KOLIBA_ApplySphericalEfficacies(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, double angle, const KOLIBA_SLUT * const alt, double fx);
KLBDC KOLIBA_SLUT *KOLIBA_ColorRoller(KOLIBA_SLUT *sLut, double imp, double angle, double atmo, double fx, double efficacy);

/* These are deprecated and will be removed in v.1.0.
   Use KOLIBA_ApplySphericalEfficacies instead.

KLBDC KOLIBA_SLUT *KOLIBA_ApplySphericalEfficaciesF(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, KOLIBA_QUINTARYCOLORS index, const KOLIBA_SLUT * const alt);
KLBDC KOLIBA_SLUT *KOLIBA_ApplySphericalEfficaciesX(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, KOLIBA_QUINTARYCOLORS index, const KOLIBA_SLUT * const alt);
// Or for that matter, by any color based on its angle in degrees:
KLBDC KOLIBA_SLUT *KOLIBA_ApplySphericalAngleEfficaciesF(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, double angle, const KOLIBA_SLUT * const alt);
KLBDC KOLIBA_SLUT *KOLIBA_ApplySphericalAngleEfficaciesX(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, double angle, const KOLIBA_SLUT * const alt);*/
// If we need to convert a KOLIBA_QUINTARYCOLORS to angle:
#ifdef NOKLINLIN
#define KOLIBA_SphericalToAngle(s)	((double)(360.0*(double)(s))/(double)KOLIBA_QuintaryColorCount)
#ifndef SPHERICAL_C
#define KOLIBA_ApplySphericalEfficaciesF(sLut,slt,index,alt)	KOLIBA_ApplySphericalEfficacies(sLut,slt,KOLIBA_SphericalToAngle(index),alt,0.0)
#define KOLIBA_ApplySphericalEfficaciesX(sLut,slt,index,alt)	KOLIBA_ApplySphericalEfficacies(sLut,slt,KOLIBA_SphericalToAngle(index),alt,1.0)
#define	KOLIBA_ApplySphericalAngleEfficaciesF(sLut,slt,angle,alt)	KOLIBA_ApplySphericalEfficacies(sLut,slt,angle,alt,0.0)
#define	KOLIBA_ApplySphericalAngleEfficaciesX(sLut,slt,angle,alt)	KOLIBA_ApplySphericalEfficacies(sLut,slt,angle,alt,1.0)
#endif
#else
inline double KOLIBA_SphericalToAngle(KOLIBA_QUINTARYCOLORS s) {
	return (360.0 * (double)s) / (double)KOLIBA_QuintaryColorCount;
}

#ifndef	SPHERICAL_C
inline KOLIBA_SLUT *KOLIBA_ApplySphericalEfficaciesF(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, KOLIBA_QUINTARYCOLORS index, const KOLIBA_SLUT * const alt) {
	return KOLIBA_ApplySphericalEfficacies(sLut, slt, KOLIBA_SphericalToAngle(index), alt, 0.0);
}

inline KOLIBA_SLUT *KOLIBA_ApplySphericalEfficaciesX(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, KOLIBA_QUINTARYCOLORS index, const KOLIBA_SLUT * const alt) {
	return KOLIBA_ApplySphericalEfficacies(sLut, slt, KOLIBA_SphericalToAngle(index), alt, 1.0);
}

inline KOLIBA_SLUT *KOLIBA_ApplySphericalAngleEfficaciesF(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, double angle, const KOLIBA_SLUT * const alt) {
	return KOLIBA_ApplySphericalEfficacies(sLut, slt, angle, alt, 0.0);
}

inline KOLIBA_SLUT *KOLIBA_ApplySphericalAngleEfficaciesX(KOLIBA_SLUT *sLut, const KOLIBA_SLUT * const slt, double angle, const KOLIBA_SLUT * const alt) {
	return KOLIBA_ApplySphericalEfficacies(sLut, slt, angle, alt, 1.0);
}
#endif
#endif

// More spherical support
KLBDC signed int KOLIBA_AngleToQuintaryIndex(double angle);
KLBDC signed int KOLIBA_TokenToQuintaryIndex(const char * const token);

// Set the farbosity of a sLut without changing its svit.
// It is defined by its center and its width and is applied
// to all six farba vertices.
//
// Default center is 0.5, default width is 1.0.
KLBDC KOLIBA_SLUT * KOLIBA_SetFarbosity(KOLIBA_SLUT *sLut, double center, double width);



/****************************************************************************/
/********************                                  **********************/
/******************** T H E  KOLIBA  F U N C T I O N S **********************/
/********************                                  **********************/
/****************************************************************************/

// Just as the KOLIBA_FLUT is the workhorse of Koliba data types along with
// KOLIBA_FLAGS, KOLIBA_ApplyXyz is the heart of Koliba functionality. It
// applies a FLUT to a KOLIBA_XYZ input vector and returns the modified output
// KOLIBA_XYZ vector.
//
// And please remember that the KOLIBA_RGB data type has the exact same
// structure as KOLIBA_XYZ, so you can use that if you prefer (you just have
// to typecast it as needed).
//
// Almost all other functions in the Koliba library either call ApplyXyz or
// serve to prepare the desired FLUT and the corresponding FLAGS.
//
// When used in an image color grading application, this function can easily
// be called many millions of times. After all, there are more than 2,000,000
// (two million) pixels in every single 1080p frame.
//
// So if you are porting the Koliba library to a system with hardware
// acceleration, this would be the function that would benefit from such
// acceleration the most.
//
// In a hypothetical Koliba co-processor hardware, this function would take
// the least number of clocks to complete.
//
// Just please remember, if you were to design such hardware, that the FLAGS
// still need to be used even if the entire function were to be designed with
// purely combinational logic. That is because while its main purpose in the
// software version is to speed things up, it also has the other purpose of
// determining which non-zero values within the FLUT to ignore to produce a
// different effect than that the FLUT was created for. For example, changing
// the FLAGS for different frames of video could produce the effect of
// blinking the lights off and on while keeping the same FLUT.

KLBDC KOLIBA_XYZ * KOLIBA_ApplyXyz(
	KOLIBA_XYZ * xyzout,			// The pointer to the output XYZ/RGB vector
	const KOLIBA_XYZ * const xyzin,	// The pointer to the input XYZ/RGB (it is OK if it is the same as the output)
	const KOLIBA_FLUT * const fLut,	// The pointer to the FLUT to apply to the input
	KOLIBA_FLAGS flags				// The flags that decide which FLUT vertices to apply
);

// Whenever our FLUT changes, we usually need to figure out the correct FLAGS.
//
// Find the fastest way to apply a FLUT to a pixel. This simply returns an
// integer which flags non-zero FLUT vertices, so they can be included in the
// computation, while the zero vertices can be skipped. (Of course, you can
// then turn off any flags you may want skipped for a special effect.)
//
// It also flags the lowest three bits regardless of the corresponding FLUT
// values because of their special nature in the FLUT-interpolating algorithm.
//
// That is why the starting value of "method" in the for loop is 7
// (= 1 | 2 | 4), the starting pointer is &(fLut->red), rather than just fLut,
// and the orring is by 8 (= 1 << 3) << i, rather than 1 << i.
//
// Keep all that in mind if you are porting the Koliba library to a different
// environment or are designing a Koliba co-processor.

KLBDC KOLIBA_FLAGS KOLIBA_FlutFlags(
	const KOLIBA_FLUT * const fLut
);

// In certain cases we may want to specify the flags directly. These
// definitions can help. Though, the first three are ignored, as we always
// start by loading the black vertex.
#define	KOLIBA_KR	0x000001	// black.r
#define	KOLIBA_KG	0x000002	// black.g
#define	KOLIBA_KB	0x000004	// black.b

#define	KOLIBA_RR	0x000008	// red.r
#define	KOLIBA_RG	0x000010	// red.g
#define	KOLIBA_RB	0x000020	// red.b

#define	KOLIBA_GR	0x000040	// green.r
#define	KOLIBA_GG	0x000080	// green.g
#define	KOLIBA_GB	0x000100	// green.b

#define	KOLIBA_BR	0x000200	// blue.r
#define	KOLIBA_BG	0x000400	// blue.g
#define	KOLIBA_BB	0x000800	// blue.b

#define	KOLIBA_YR	0x001000	// yellow.r
#define	KOLIBA_YG	0x002000	// yellow.g
#define	KOLIBA_YB	0x004000	// yellow.b

#define	KOLIBA_MR	0x008000	// magenta.r
#define	KOLIBA_MG	0x010000	// magenta.g
#define	KOLIBA_MB	0x020000	// magenta.b

#define	KOLIBA_CR	0x040000	// cyan.r
#define	KOLIBA_CG	0x080000	// cyan.g
#define	KOLIBA_CB	0x100000	// cyan.b

#define	KOLIBA_WR	0x200000	// white.r
#define	KOLIBA_WG	0x400000	// white.g
#define	KOLIBA_WB	0x800000	// white.b

// Here are some common ones. Please remember we normally want the lowest
// three flags set even if the FLUT has zeroes in the black vertex. It is not
// a big deal, it just speeds up processing in a miniscule way (but when you
// get a tiny slowdown in millions of pixels in some 100000 frames of video,
// even a miniscule speedup matters).
//
// Therefore, the black flags should only be turned off if we want to ignore
// non-zero values in the black vertex for some special effect.
//
// For example, if we set the flags entirely to zero, we will produce a pure
// black frame regardless of the contents of the FLUT.
//
// On the other hand, if we turn off all but the black flags (so, flags = 7),
// we will produce a solid color whose value is in the black vertex.
#define	KOLIBA_BlackFlutFlags	(KOLIBA_KR | KOLIBA_KG | KOLIBA_KB)
#define	KOLIBA_RgbFlutFlags	KOLIBA_BlackFlutFlags
#define	KOLIBA_DefaultFlutFlags	(KOLIBA_RR | KOLIBA_GG | KOLIBA_BB)
#define	KOLIBA_1DFlutFlags	(KOLIBA_DefaultFlutFlags  | KOLIBA_BlackFlutFlags)
#define	KOLIBA_IdentityFlutFlags	KOLIBA_1DFlutFlags
#define	KOLIBA_GrayFlutFlags	(0x000FF8)
#define	KOLIBA_GreyFlutFlags	KOLIBA_GrayFlutFlags
#define	KOLIBA_MatrixFlutFlags	(KOLIBA_GrayFlutFlags | KOLIBA_BlackFlutFlags)
#define	KOLIBA_AllFlutFlags		(0xFFFFFF)

// It may be useful to determine if a FLUT is Identity FLUT. It, for example,
// alows us to just quickly copy all the input pixels to the output pixels.

KLBDC bool KOLIBA_IsIdentityFlut(
	const KOLIBA_FLUT * const fLut
);

// And sometimes we need to see if a SLUT is Identity SLUT.
KLBDC bool KOLIBA_IsIdentitySlut(const KOLIBA_SLUT * const sLut);

// Apply a chain of n FLUTs to an XYZ vector. If n is zero, just copy the
// input to the output. In that case, FFLUT may be NULL because it is not used
// anyway.
//
// (Recall that FFLUT contains both a FLUT and the corresponding FLAGS, so
// each FLUT in the chain can come with different FLAGS.)

KLBDC KOLIBA_XYZ * KOLIBA_PolyXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const KOLIBA_FFLUT * const ffLut,
	unsigned int n
);

// Apply a chain of n FLUTs to an XYZ vector, process the result by an
// external function, then apply a second chain of m FLUTs to whatever the
// external changes the result to.
//
// The FFLUT array must be of n+m size, with the first n FLUTs applied before
// calling the external function, and the next m FLUTs applied afterwards.
//
// The external function accepts the XYZ vector (which is both its input and
// its output), and a pointer to any number/type of parameters it needs.

KLBDC KOLIBA_XYZ * KOLIBA_ExternalXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const KOLIBA_FFLUT * const ffLut,
	unsigned int n,
	unsigned int m,
	KOLIBA_EXTERNAL ext,
	const void * const params
);


// A simple example function of the KOLIBA_EXTERNAL variety. Its params is
// expected to be a pointer of the type KOLIBA_XYZ containing the gamma.x,
// gamma.y, and gamma.z. The xyz.x, xyz.y, and xyz.z are raised to the power
// of their corresponding gammas.

KLBDC KOLIBA_XYZ * KOLIBA_Gamma(
	KOLIBA_XYZ * xyz,
	const void * const params
);

// Before you call that you may want to call this once. It will adjust q into
// params based on whether the members of q are negative or non-negative.
//
// You only need to call it once after any q parameter changes, then pass
// params to KOLIBA_Gamma, or rather KOLIBA_ExternalXyz, as many times as
// needed.

KLBDC KOLIBA_XYZ * KOLIBA_PrepareGammaParameters(
	KOLIBA_XYZ * params,
	const KOLIBA_XYZ * const q
);


// Typically, we present the user with a variety of options, which we use to
// produce a SLUT or a PLUT or a MATRIX or a PALETTE or some other structure
// that we need to convert to a FLUT.
//
// We only need to do that whenever some data in said structure has changed.
// We definitely do NOT need to run the conversion if no data has changed, so
// we do not produce a FLUT before every pixel. There is an exception when we
// are applying a non-simple LUT with compression applied.

// We convert a SLUT to a FLUT indirectly by using VERTICES. We typically only
// need to convert a SLUT to VERTICES once because even if the data in the
// SLUT changes, as long as the SLUT remains at the same place in the memory,
// the VERTICES are still correct.
//
// Once we have the VERTICES, we can use them to convert the respective SLUT
// into a FLUT as often as we need.

KLBDC KOLIBA_VERTICES * KOLIBA_SlutToVertices(
	KOLIBA_VERTICES * vertices,
	KOLIBA_SLUT * const sLut
);

// Scale the sLut factors in a FLUT so that the results
// of applying them does not have to be scaled up for
// every channel of every pixel.
//
// While the input of the respective KOLIBA_ApplyXyz()
// still expects the values in the nominal 0-1 range,
// the output of a scaled FLUT will be in the 0-n range,
// where n is the scale factor applied here.
//
// This saves processing time because with the most common
// pixel size of using 8-bit integer bytes for the channels
// converting them to the 0-1 range is a matter of a small
// lookup table, but we avoid multiplying them back by 255
// (or whatever factor we use) and only need to convert them
// back to integers, which modern processors do fast.
//
// The scaling should normally be the last step before
// pixel processing because it breaks such things as
// checking for an identity FLUT, it makes interpolating
// FLUTS harder, etc.

KLBDC KOLIBA_FLUT * KOLIBA_ScaleFlut(
	KOLIBA_FLUT *scaled,
	const KOLIBA_FLUT * const fLut,
	double factor
);

// We can calculate KOLIBA_FLUT from KOLIBA_VERTICES.
KLBDC KOLIBA_FLUT * KOLIBA_ConvertSlutToFlut(
	KOLIBA_FLUT * fLut,
	const KOLIBA_VERTICES * const vertices
);

// We can also convert a FLUT back to a SLUT.
KLBDC KOLIBA_SLUT * KOLIBA_ConvertFlutToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_FLUT * const f
);

// We can convert a PLUT directly to a SLUT, too.
KLBDC KOLIBA_SLUT * KOLIBA_ConvertPlutToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_PLUT * const pLut
);

// We can even convert a SLUT to a PLUT...
KLBDC KOLIBA_PLUT * KOLIBA_ConvertSlutToPlut(
	KOLIBA_PLUT * pLut,
	const KOLIBA_SLUT * const sLut
);

// ...And reset a PLUT to its defaults
KLBDC KOLIBA_PLUT * KOLIBA_ResetPlut(
	KOLIBA_PLUT * pLut
);

// We can convert VERTICES back to SLUT as well.
KLBDC KOLIBA_SLUT *KOLIBA_VerticesToSlut(
	KOLIBA_SLUT * sLut,
	KOLIBA_VERTICES * const v
);

// Find out if a 3D SLUT is really just a 1D Lut.
KLBDC int KOLIBA_SlutIs1D(
	const KOLIBA_SLUT * const sLut
);

// Here is a VERTICES version of SlutIs1D.
KLBDC int KOLIBA_VerticesIs1D(
	const KOLIBA_VERTICES * const vert
);

// An effect may be applied to a portion of a SLUT based on the KOLIBA_SLUT*
// flags, such as produced by the conversion of a fundmal to an unsigned char.
// You can do that by accessing the KOLIBA_FundamentalMalletFlags array
// directly. Better yet, call KOLIBA_Fundmal(), which returns zero if you pass
// an invalid fundmal argument to it.

KLBDC unsigned char KOLIBA_Fundmal(
	fundmal f
);

// While many functions accept the SLUT* flags as a parameter, KOLIBA_Fundmal
// conversion allows you to copy any SLUT effect to just a portion of a SLUT.
//
// Copy select vertices from one SLUT to another. The rest of the vertices are
// left unchanged, so make sure to initialize the output to some known state
// before calling this for the first time.

KLBDC KOLIBA_SLUT * KOLIBA_SlutSelect
	(KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const sLut,
	unsigned char flags
);

// Conditionally invert vertices of a sLut.
KLBDC KOLIBA_SLUT * KOLIBA_InvertSlutVertices(
	KOLIBA_SLUT * sLut,
	unsigned char flags
);

// Conditionally apply gain to vertices of a sLut.
KLBDC KOLIBA_SLUT * KOLIBA_ApplyGainToSlutVertices(
	KOLIBA_SLUT * sLut,
	unsigned char flags,
	double gain
);

// Conditionally apply lift to vertices of a sLut.
KLBDC KOLIBA_SLUT * KOLIBA_ApplyLiftToSlutVertices(
	KOLIBA_SLUT * sLut,
	unsigned char flags,
	double lift
);

// Conditionally add offset to vertices of a sLut.
KLBDC KOLIBA_SLUT * KOLIBA_AddOffsetToSlutVertices(
	KOLIBA_SLUT * sLut,
	unsigned char flags,
	double offset
);

#ifdef	NOKLINLIN

#define	KOLIBA_ApplyContrastToSlutVertices(sLut,color,flags,contrast)	KOLIBA_InterpolateSlutVertices((sLut),(color),(sLut),-(contrast),(flags))
#define	KOLIBA_ApplyEfficacyToSlutVertices(sLut,flags,efficacy)	KOLIBA_InterpolateSlutVertices((sLut),(sLut),&KOLIBA_IdentitySlut,(efficacy),(flags))
#define	KOLIBA_ApplySaturationToSlutVertices(sLut,gLut,flags,saturation)	KOLIBA_InterpolateSlutVertices((sLut),(sLut),(gLut),(saturation),(flags))

#else

inline KOLIBA_SLUT * KOLIBA_ApplyContrastToSlutVertices(KOLIBA_SLUT * sLut, const KOLIBA_SLUT * const color, unsigned char flags, double contrast) {
	return KOLIBA_InterpolateSlutVertices(sLut, color, sLut, -contrast, flags);
}

inline KOLIBA_SLUT * KOLIBA_ApplyEfficacyToSlutVertices(KOLIBA_SLUT * sLut, unsigned char flags, double efficacy) {
	return KOLIBA_InterpolateSlutVertices(sLut, sLut, &KOLIBA_IdentitySlut, efficacy, flags);
}

inline KOLIBA_SLUT * KOLIBA_ApplySaturationToSlutVertices(KOLIBA_SLUT * sLut, const KOLIBA_SLUT * const grayLut, unsigned char flags, double saturation) {
	return KOLIBA_InterpolateSlutVertices(sLut, sLut, grayLut, saturation, flags);
}

#endif

// Convert a monofarba into a SLUT. Vertices listed in the flags byte will
// have the primary saturation, the rest of them the secondary saturation.
KLBDC KOLIBA_SLUT * KOLIBA_MonoFarbaToSlut(
	KOLIBA_SLUT * output,
	const KOLIBA_RGB * const gray,	// May be NULL.
	double primarysaturation,
	double secondarysaturation,
	uint8_t flags
);

#ifdef NOKLINLIN
#define	KOLIBA_MonoFarbaToFlut(output,gray,primary,secondary,flags)	do {\
	KOLIBA_SLUT sLut_0_0_0_0_1;\
	KOLIBA_VERTICES vert_0_0_0_0_2;\
	KOLIBA_SlutToVertices(&vert_0_0_0_0_2,KOLIBA_MonoFarbaToSlut(&sLut_0_0_0_0_1,gray,primary,secondary,flags));\
	KOLIBA_ConvertSlutToFlut(output,&vert_0_0_0_0_2);\
} while(0)
#else
inline KOLIBA_FLUT * KOLIBA_MonoFarbaToFlut(KOLIBA_FLUT * output, const KOLIBA_RGB * gray, double primary, double secondary, unsigned char flags)	{
	KOLIBA_SLUT sLut;
	KOLIBA_VERTICES vert;

	KOLIBA_SlutToVertices(&vert, KOLIBA_MonoFarbaToSlut(&sLut, gray, primary, secondary, flags));
	return KOLIBA_ConvertSlutToFlut(output,&vert);
}
#endif

// This one often looks good.
#define	KOLIBA_RedMonoFarbaToSlut(sLut)	KOLIBA_MonoFarbaToSlut(sLut,NULL,1.25,-0.25,KOLIBA_SLUTRED)
#define	KOLIBA_RedMonoFarbaToFlut(fLut)	KOLIBA_MonoFarbaToFlut(fLut,NULL,1.25,-0.25,KOLIBA_SLUTRED)

// Convert grayscale factors to a SLUT or directly to a FLUT. If the factors
// are NULL, use Rec. 2020 instead. If the factors add up to 0, set them all
// to 1/3.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertGrayToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_RGB * const gray
);

KLBDC KOLIBA_FLUT * KOLIBA_ConvertGrayToFlut(
	KOLIBA_FLUT * fLut,
	const KOLIBA_RGB * const gray
);

#define	KOLIBA_ConvertGreyToSlut(s,g)	KOLIBA_ConvertGrayToSlut(s,g)
#define	KOLIBA_ConvertRecToSlut(s,r)	KOLIBA_ConvertGrayToSlut(s,r)
#define	KOLIBA_ConvertGreyToFlut(f,g)	KOLIBA_ConvertGrayToFlut(f,g)
#define	KOLIBA_ConvertRecToFlut(f,r)	KOLIBA_ConvertGrayToFlut(f,r)

// Convert an RGB color to a FLUT. If no color is specified, use black.
KLBDC KOLIBA_FLUT * KOLIBA_ConvertRgbToFlut(
	KOLIBA_FLUT * fLut,
	const KOLIBA_RGB * const rgb
);

// Convert an RGB color to a SLUT. If no color is specified, use black.
KLBDC KOLIBA_SLUT * KOLIBA_ConvertRgbToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_RGB * const rgb
);

// Because some cube LUT plug-ins get confused by very small values, turn
// those to 0. The sole argument to these functions is both, the input LUT
// and the output LUT.
KLBDC KOLIBA_SLUT * KOLIBA_FixSlut(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_FLUT * KOLIBA_FixFlut(
	KOLIBA_FLUT * fLut
);

// It is useful to know that we can think of a SLUT as consisting of vertices
// we shall call the "svit" and vertices we shall call the "farba".
//
// The svit consists of the black and white vertex, the farba of all other
// vertices. The two groups affect pixels in different ways.
//
// The svit is somewhat similar to the luma in YCrCb pixels, and the farba is
// similar to its chroma. But they are not the same, indeed, they control
// colors in ways luma and chroma cannot. After all, luma and chroma can be
// fully represented by our MATRIX structure. But the MATRIX is only a subset
// of a LUT (we can convert every KOLIBA_MATRIX to KOLIBA_SLUT but we cannot
// convert every KOLIBA_SLUT to KOLIBA_MATRIX without some loss of information
// the LUT contains).
//
// Nevertheless, they are similar in that the svit controls mostly the
// brightness and the contrast of pixels, while the farba controls mostly
// their color aspects. This allows to change the brightness and contrast
// of pixels without affecting their color aspects simply by changing the
// black and the white vertex (especially the black one, though). And we can
// affect the color aspects of the pixels without messing up the brightness
// and contrast simply by changing some or all of the farba vertices while
// leaving the svit vertices untouched.
//
// And if we make a mistake, we can reset the svit or the farba. Or both, just
// make sure to discard the svit first. Though just resetting the SLUT
// accomplishes the same faster.

// Discard svit from a SLUT by resetting it (the svit).
KLBDC KOLIBA_SLUT * KOLIBA_DiscardSlutSvit(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input
);

// Discard the farba from a SLUT.
KLBDC KOLIBA_SLUT * KOLIBA_DiscardSlutFarba(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input
);

// Reset a SLUT to default contents.
KLBDC KOLIBA_SLUT * KOLIBA_ResetSlut(
	KOLIBA_SLUT * sLut
);

// Reset a SLUT vertex to default contents.
//
// N.B.: We are passing the pointer to the SLUT to these functions, not a
// pointer to the specific vertex.

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutBlack(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutBlue(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutGreen(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutCyan(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutRed(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutMagenta(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutYellow(
	KOLIBA_SLUT * sLut
);

KLBDC KOLIBA_SLUT * KOLIBA_ResetSlutWhite(
	KOLIBA_SLUT * sLut
);

// We can convert a MATRIX directly to a FLUT, or even to a SLUT if needed.
KLBDC KOLIBA_FLUT * KOLIBA_ConvertMatrixToFlut(
	KOLIBA_FLUT * fLut,
	const KOLIBA_MATRIX * const mat
);

KLBDC KOLIBA_SLUT * KOLIBA_ConvertMatrixToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_MATRIX * const mat
);

// We can test if a FLUT or a SLUT could be a matrix. The result is boolean
// but we declare the functions as returning int to be compatible with all
// C compilers.

KLBDC int KOLIBA_FlutIsMatrix(
	const KOLIBA_FLUT * const f
);

KLBDC int KOLIBA_SlutIsMatrix(
	const KOLIBA_SLUT * const s
);

// And a VERTICES version of SlutIsMatrix.
KLBDC int KOLIBA_VerticesIsMatrix(
	const KOLIBA_VERTICES * const s
);

// This is the inverse of KOLIBA_ConvertMatrixToFlut (and of
// KOLIBA_ConvertMatrixToSlut), but only if the FLUT (and SLUT) really is a
// matrix.
//
// Otherwise, it extracts a matrix based on prime farba but ignoring secondary
// farba, which is not an inverse of KOLIBA_ConvertMatrixTo[FS]lut, but
// perhaps could be used as a special effect.
//
// Though anding the FLAGS with KOLIBA_MatrixFlutFlags has the same effect on
// a FLUT without needing to rewrite the FLUT.

KLBDC KOLIBA_MATRIX * KOLIBA_ConvertFlutToMatrix(
	KOLIBA_MATRIX * mat,
	const KOLIBA_FLUT * const fLut
);

KLBDC KOLIBA_MATRIX * KOLIBA_ConvertSlutToMatrix(
	KOLIBA_MATRIX * mat,
	const KOLIBA_SLUT * const sLut
);

// We can reset a matrix or one of its rows to the identity (default).

KLBDC KOLIBA_MATRIX * KOLIBA_ResetMatrix(
	KOLIBA_MATRIX * matrix
);

KLBDC KOLIBA_MATRIX * KOLIBA_ResetMatrixRed(
	KOLIBA_MATRIX * matrix
);

KLBDC KOLIBA_MATRIX * KOLIBA_ResetMatrixGreen(
	KOLIBA_MATRIX * matrix
);

KLBDC KOLIBA_MATRIX * KOLIBA_ResetMatrixBlue(
	KOLIBA_MATRIX * matrix
);

// Normalizes a matrix row in place, but only if wade is not zero.

KLBDC KOLIBA_ROW * KOLIBA_NormalizeMatrixRow(KOLIBA_ROW *row, unsigned int wade);

// Create a matrix to control top and bottom values,
// roughly corresponding to gain and lift.

KLBDC KOLIBA_MATRIX * KOLIBA_MatrixSpan(KOLIBA_MATRIX *mat, KOLIBA_RGB *top, KOLIBA_RGB *bottom);

// We can apply a matrix to another matrix, i.e., multiply them. The
// multiplier modifies the multiplicand here, not the other way.
//
// The beauty of matrix multiplication is that it allows us to apply any
// number of matrices in a row all at once by multiplying them first.
//
// A matrix represents a function. For example, suppose we have three
// matrices A, B, and C, each representing a function, a, b, c.
//
// To apply function a to pixel p, i.e., to calculate p' = a(p), we
// multiply pixel p by matrix A, i.e., p' = A*p. If we then want to apply
// function b to the modifies pixel p', i.e., p'' = b(p'), or p'' = B*p'.
// And finally, to apply function c to p'' we get p''' = c(p''), i.e.,
// p''' = C*p''.
//
// Mathematically, it is equivalent to p''' = c(b(a(p))). More importantly,
// it is the same as p''' = C*B*A*p. So, we can produce a matrix M = C*B*A,
// and then just apply that to all the pixels, as M*p = C*B*A*p.
//
// Which is to say,
//
//		KOLIBA_MATRIX A, B, C, M;
//		KOLIBA_FLUT fLut;
//
//		KOLIBA_MultiplyMatrices(&M, &A, &B);
//		KOLIBA_MultiplyMatrices(&M, &M, &C);
//		KOLIBA_ConvertMatrixToFlut(&fLut, &M);
//
// And then we just loop through KOLIBA_ApplyXyz(&output, &input, &fLut,
// KOLIBA_MatrixFlutFlags);

KLBDC KOLIBA_MATRIX * KOLIBA_MultiplyMatrices(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const multiplicand,
	const KOLIBA_MATRIX * const multiplier
);

// "Multiply" SLUTs. This operation is only analogous to matrix multiplication
// but is not as precise (because a LUT is not a function, only an
// approximation of a function).

KLBDC KOLIBA_SLUT * KOLIBA_MultiplySluts(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const multiplicand,
	const KOLIBA_SLUT * const multiplier
);

// Flutter is derived from matrix multiplication, then modified. It was
// originally called "KOLIBA_MultiplyFluts", then I decided to call it
// KOLIBA_Flutter instead.

KLBDC KOLIBA_FLUT * KOLIBA_Flutter(
	KOLIBA_FLUT * output,
	const KOLIBA_FLUT * const modificand,
	const KOLIBA_FLUT * const modifier
);

// Lift the channels of a matrix, each channel separately. If a modificand
// matrix is not specified, use I (identity matrix) as input.

KLBDC KOLIBA_MATRIX * KOLIBA_MatrixLift(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const input,
	const KOLIBA_VERTEX * const lifts
);

// Similarly for the gains.
KLBDC KOLIBA_MATRIX * KOLIBA_MatrixGain(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const input,
	const KOLIBA_VERTEX * const gains
);

// A channel blend is really just a way of presenting a MATRIX in a
// user-friendly way.
//
// To reset channel blend or one of its rows,

KLBDC KOLIBA_CHANNELBLEND * KOLIBA_ResetChannelBlend(
	KOLIBA_CHANNELBLEND * blend
);

KLBDC KOLIBA_CHANNELBLEND * KOLIBA_ResetChannelBlendRed(
	KOLIBA_CHANNELBLEND * blend
);

KLBDC KOLIBA_CHANNELBLEND * KOLIBA_ResetChannelBlendGreen(
	KOLIBA_CHANNELBLEND * blend
);

KLBDC KOLIBA_CHANNELBLEND * KOLIBA_ResetChannelBlendBlue(
	KOLIBA_CHANNELBLEND * blend
);

KLBDC KOLIBA_MATRIX * KOLIBA_ConvertChannelBlendToMatrix(
	KOLIBA_MATRIX * matrix,
	const KOLIBA_CHANNELBLEND * const blend
);

KLBDC KOLIBA_CHANNELBLEND * KOLIBA_ConvertMatrixToChannelBlend(
	KOLIBA_CHANNELBLEND * blend,
	const KOLIBA_MATRIX * const matrix
);

// So, should we not be able to convert a channel blend to a FLUT? Well, if
// you insist...

#ifdef	NOKLINLIN
#define	KOLIBA_ConvertChannelBlendToFlut(fLut,blend)	do {\
	KOLIBA_MATRIX m_a_t_13_05;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ConvertChannelBlendToMatrix(&m_a_t_13_05, blend));\
} while(0)
#define	KOLIBA_ConvertChannelBlendToSlut(sLut,blend)	do {\
	KOLIBA_MATRIX m_a_t_37_18;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ConvertChannelBlendToMatrix(&m_a_t_37_18, blend));\
} while(0)
#else
inline KOLIBA_FLUT * KOLIBA_ConvertChannelBlendToFlut(KOLIBA_FLUT * fLut, const KOLIBA_CHANNELBLEND * blend) {
	KOLIBA_MATRIX mat;
	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ConvertChannelBlendToMatrix(&mat, blend));
}

inline KOLIBA_SLUT * KOLIBA_ConvertChannelBlendToSlut(KOLIBA_SLUT * sLut, const KOLIBA_CHANNELBLEND * blend) {
	KOLIBA_MATRIX mat;
	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ConvertChannelBlendToMatrix(&mat, blend));
}
#endif

// One effect that is quite popular with filmmakers and hated by some armchair
// critics is reducing the image to just two complementary colors. You may
// think it quite difficult to achieve given our RGB vision. In reality, this
// effect is very easy to accomplish. All we need is a gray complement matrix.
//
// When applied to any pixel, the GC matrix leaves one RGB channel unmodified
// (this is typically the red channel but another channel may be used for a
// special effect). But it replaces the other two channels with what all three
// would be if the pixel were turned gray (so, by the luminance of the pixel).
//
// Let us assume it is the red channel we are leaving unchanged. If so, then
// one of three possibilities will happen in any pixel so processed:
//
//	(1) The value of the red channel is greater than the value of the other
//		two. The red channel will dictate the pixel should be red. It will
//		also dictate its brightness. The other two will dictate the
//		chrominance (or saturation if you prefer).
//
//	(2) The value of the red channel will be the same as the value of the
//		other two channels. As such the chrominance of the pixel will be
//		zero, so the pixel will appear gray with its brightness being the
//		same as all three channels.
//
//	(3) The value of the red channel is lesser than that of the other two.
//		So the other two channels will dictate they should be the complement
//		of red (which technically is cyan but it will probably appear a
//		little greener). They will also dictate the brightness of the pixel.
//		The red channel will dictate the chrominance (or saturation) of
//		the pixel but not its color (or hue).
//
// This is a very simple method of producing a color and its complement. You
// just pass it a Rec (cannot be NULL and the sum of three values in it may
// not be zero) and the number of the channel you want to complement (0=red,
// 1=green, 2=blue). No other options for this quick and simple solution.

KLBDC KOLIBA_MATRIX * KOLIBA_GrayComplementMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_RGB * const rec,
	unsigned int channel
);
#define	KOLIBA_GreyComplementMatrix(p,q,r)	KOLIBA_GrayComplementMatrix(p,q,r)

#ifdef	NOKLINLIN
#define	KOLIBARedComplementMatrix(output,rec)	KOLIBA_GrayComplementMatrix((output), (rec), 0)
#define	KOLIBAGreenComplementMatrix(output,rec)	KOLIBA_GrayComplementMatrix((output), (rec), 1)
#define	KOLIBABlueComplementMatrix(output,rec)	KOLIBA_GrayComplementMatrix((output), (rec), 2)
#else
inline KOLIBA_MATRIX * KOLIBARedComplementMatrix(KOLIBA_MATRIX *output, KOLIBA_RGB *rec) {
	return KOLIBA_GrayComplementMatrix(output, rec, 0);
}

inline KOLIBA_MATRIX * KOLIBAGreenComplementMatrix(KOLIBA_MATRIX *output, KOLIBA_RGB *rec) {
	return KOLIBA_GrayComplementMatrix(output, rec, 1);
}

inline KOLIBA_MATRIX * KOLIBABlueComplementMatrix(KOLIBA_MATRIX *output, KOLIBA_RGB *rec) {
	return KOLIBA_GrayComplementMatrix(output, rec, 2);
}
#endif


// Any of the YCbCr adjustments can be accomplished using matrices. We have
// several types of chroma-adjusting matrices declared above in the data type
// section of this header file.

// Reset a chroma matrix to default contents.
KLBDC KOLIBA_CHROMA * KOLIBA_ResetChroma(
	KOLIBA_CHROMA * chr
);

// Set the chroma model.
KLBDC KOLIBA_RGB * KOLIBA_SetChromaModel(
	KOLIBA_RGB * outmodel,
	const KOLIBA_RGB * const inmodel
);

// Produce a matrix to control chroma angle, saturation and black and white
// levels. The angle has to be in DEGREES since that is easier for the user to
// specify than radians.
KLBDC KOLIBA_MATRIX * KOLIBA_ChromaMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_CHROMA * const chroma,
	const KOLIBA_RGB * const model
);

#ifdef	NOKLINLIN

#define	KOLIBA_ConvertChromaMatrixToFlut(fLut,chroma,model)	do {\
	KOLIBA_MATRIX m_a_t_29_15;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ChromaMatrix(&m_a_t_29_15, chroma, model));\
} while(0)

#define	KOLIBA_ConvertChromaMatrixToSlut(sLut,chroma,model)	do {\
	KOLIBA_MATRIX m_a_t_87_08;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ChromaMatrix(&m_a_t_87_08, chroma, model));\
} while(0)

#else

inline KOLIBA_FLUT * KOLIBA_ConvertChromaMatrixToFlut(KOLIBA_FLUT * fLut, const KOLIBA_CHROMA * chroma, const KOLIBA_RGB * model) {
	KOLIBA_MATRIX mat;
	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ChromaMatrix(&mat, chroma, model));
}

inline KOLIBA_SLUT * KOLIBA_ConvertChromaMatrixToSlut(KOLIBA_SLUT * sLut, const KOLIBA_CHROMA * chroma, const KOLIBA_RGB * model) {
	KOLIBA_MATRIX mat;
	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ChromaMatrix(&mat, chroma, model));
}

#endif

// This is how the chromat is converted to a matrix. The chromat, or chromatic
// matrix, is the simplest of the chroma structures and can be used to change
// the angle, the saturation, and the black and white points.
KLBDC KOLIBA_MATRIX * KOLIBA_ChromaticMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_CHROMAT * const chromat
);

// And here is how the chromat is converted to a SLUT and a FLUT.
#ifdef	NOKLINLIN

#define	KOLIBA_ConvertChromatToSlut(sLut,chromat) do {\
	KOLIBA_MATRIX m_a_t___QX_87;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ChromaticMatrix(&m_a_t___QX_87, chromat));\
} while(0)

#define	KOLIBA_ConvertChromatToFlut(fLut,chromat) do {\
	KOLIBA_MATRIX m_a_t_RM__2o;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ChromaticMatrix(&m_a_t_RM__2o, chromat));\
} while(0)

#else

inline KOLIBA_SLUT * KOLIBA_ConvertChromatToSlut(KOLIBA_SLUT *sLut, const KOLIBA_CHROMAT *chromat) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_ChromaticMatrix(&mat, chromat));
}

inline KOLIBA_FLUT * KOLIBA_ConvertChromatToFlut(KOLIBA_FLUT *fLut, const KOLIBA_CHROMAT *chromat) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_ChromaticMatrix(&mat, chromat));
}

#endif

// Reset a chromatic matrix to default contents, plus set the model. If no
// model is specified, do not change it and only reset the rest.

KLBDC KOLIBA_CHROMAT * KOLIBA_ResetChromaticMatrix(
	KOLIBA_CHROMAT * chrm,
	const KOLIBA_RGB * const model
);

// Reset a chromatic matrix to default contents, plus set the model to Rec2020.

KLBDC KOLIBA_CHROMAT * KOLIBA_ResetChromat(
	KOLIBA_CHROMAT * chrm
);

// Set the model inside a chromatic matrix without changing the chromatic
// matrix itself.

KLBDC KOLIBA_CHROMAT * KOLIBA_SetChromaticMatrixModel(
	KOLIBA_CHROMAT * chrm,
	const KOLIBA_RGB * const model
);

// Create a dichromatic matrix, defined by a chroma from an angle (in degrees)
// and its complement. Optionally, the matrix may be normalized to preserve
// the brightness. Further, the entire matrix may still be rotated, so the two
// (apparent) chromas, while exact complements of each other, can produce any
// two complements, not just red/not-red, green/not-green, or blue/not-blue.
//
// Its dichromacy is actually an illusion. Its effect is similar to that of
// the gray complement matrix but has many more options.
//
// The dichromatic matrix applies a chromatic matrix to one and only one color
// channel and turns the other two to what they would be in a grayscale
// matrix. Our brain then interprets some pixels as being of the color
// dictated by the one channel, and other pixels as being the complement of
// that color.
//
// This allows you to produce, for example, the popular maroon-and-teal color
// scheme. Though you can pick any one of the three channels (0=red, 1=green,
// 2=blue, any other value is rejected as error), and rotate them to any angle
// you want, plus it is sometimes a good idea to increase the saturation of
// that channel to balance out the illusion (so both opposites seem equally
// represented).
//
// So why name it dichromatic when there is only one chroma? Because calling
// it monochromatic would be confusing, as the word monochrome means something
// else. And because the desired effect, as illusory as it may be, is indeed
// perceptionally dichromatic.

KLBDC KOLIBA_MATRIX * KOLIBA_DichromaticMatrix(
	KOLIBA_MATRIX * mat,
	const KOLIBA_DICHROMA * const dicr,
	unsigned int normalize,
	unsigned int channel
);

#ifdef	NOKLINLIN

#define	KOLIBA_ResetDichromaticMatrix(dicr)	do {\
	KOLIBA_ResetChromat(&dicr->chr);\
	dicr->rotation=0.0;\
	dicr->efficacy=1.0;\
	dicr;\
} while(0)

#define	KOLIBA_ConvertDichromaticMatrixToSlut(sLut,dicr,normalize,channel) do {\
	KOLIBA_MATRIX m_a_t_h___QX_87;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_DichromaticMatrix(&m_a_t_h___QX_87, dicr, normalize, channel));\
} while(0)

#define	KOLIBA_ConvertDichromaticMatrixToFlut(fLut,dicr,normalize,channel) do {\
	KOLIBA_MATRIX m_a_t_h_RM__2o;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_DichromaticMatrix(&m_a_t_h_RM__2o, dicr, normalize, channel));\
} while(0)

#else

inline KOLIBA_DICHROMA * KOLIBA_ResetDichromaticMatrix(KOLIBA_DICHROMA *dicr) {
	KOLIBA_ResetChromat(&dicr->chr);
	dicr->rotation = 0.0;
	dicr->efficacy = 1.0;
	return dicr;
}

inline KOLIBA_SLUT * KOLIBA_ConvertDichromaticMatrixToSlut(KOLIBA_SLUT *sLut, const KOLIBA_DICHROMA *dicr, unsigned int normalize, unsigned int channel) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_DichromaticMatrix(&mat, dicr, normalize, channel));
}

inline KOLIBA_FLUT * KOLIBA_ConvertDichromaticMatrixToFlut(KOLIBA_FLUT *fLut, const KOLIBA_DICHROMA *dicr, unsigned int normalize, unsigned int channel) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_DichromaticMatrix(&mat, dicr, normalize, channel));
}

#endif


// Create an anachromatic matrix, defined by two chromas and one gray channel.
// In this case, the channel argument selects the gray channel. The first
// chroma (chr[0]) affects the first non-gray channel (so either red or green)
// and the second chroma (chr[1]) the remaining non-gray channel (green or
// blue). The two chromas can be manipulated independently so the anachromatic
// matrix allows more flexibility of color manipulation than the chromatic or
// the dichromatic matrix.

KLBDC KOLIBA_MATRIX * KOLIBA_AnachromaticMatrix(
	KOLIBA_MATRIX * mat,
	const KOLIBA_ANACHROMA * const ancr,
	unsigned int normalize,
	unsigned int channel
);

#ifdef	NOKLINLIN

#define	KOLIBA_ConvertAnachromaticMatrixToSlut(sLut,ancr,normalize,channel) do {\
	KOLIBA_MATRIX m_a_t_a___QX_87;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_AnachromaticMatrix(&m_a_t_a___QX_87, ancr, normalize, channel));\
} while(0)

#define	KOLIBA_ConvertAnachromaticMatrixToFlut(fLut,ancr,normalize,channel) do {\
	KOLIBA_MATRIX m_a_t_a_RM__2o;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_AnachromaticMatrix(&m_a_t_a_RM__2o, ancr, normalize, channel));\
} while(0)

#else

inline KOLIBA_SLUT * KOLIBA_ConvertAnachromaticMatrixToSlut(KOLIBA_SLUT *sLut, const KOLIBA_ANACHROMA *ancr, unsigned int normalize, unsigned int channel) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_AnachromaticMatrix(&mat, ancr, normalize, channel));
}

inline KOLIBA_FLUT * KOLIBA_ConvertAnachromaticMatrixToFlut(KOLIBA_FLUT *fLut, const KOLIBA_ANACHROMA *ancr, unsigned int normalize, unsigned int channel) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_AnachromaticMatrix(&mat, ancr, normalize, channel));
}

#endif

// Create a diachromatic matrix, defined by three chromas. This one is the
// most flexible of the chromatic matrices. So why not just use this one? You
// certainly can, but we are following the parsimony principle set out near
// the top of this file, and let us just say that using this one for a simple
// chromatic rotation, to name just one example, would equal entia sine
// necessitate multiplicata.
//
// This one also allows us to determine which specific channels to normalize
// by setting the normalize argument to a combination of the normalize flags.

#define	NORMALIZE_RED	1
#define	NORMALIZE_GREEN	2
#define	NORMALIZE_BLUE	4
#define	NORMALIZE_ALL	(NORMALIZE_RED | NORMALIZE_GREEN | NORMALIZE_BLUE)

KLBDC KOLIBA_MATRIX * KOLIBA_DiachromaticMatrix(
	KOLIBA_MATRIX * mat,
	const KOLIBA_DIACHROMA * const dacr,
	unsigned int normalize
);

#ifdef	NOKLINLIN

#define	KOLIBA_ConvertDiachromaticMatrixToSlut(sLut,ancr,normalize) do {\
	KOLIBA_MATRIX m_a_t_d___QX_87;\
	KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_DiachromaticMatrix(&m_a_t_d___QX_87, ancr, normalize));\
} while(0)

#define	KOLIBA_ConvertDiachromaticMatrixToFlut(fLut,ancr,normalize) do {\
	KOLIBA_MATRIX m_a_t_d_RM__2o;\
	KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_DiachromaticMatrix(&m_a_t_d_RM__2o, ancr, normalize));\
} while(0)

#else

inline KOLIBA_SLUT * KOLIBA_ConvertDiachromaticMatrixToSlut(KOLIBA_SLUT *sLut, const KOLIBA_DIACHROMA *dacr, unsigned int normalize) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToSlut(sLut, KOLIBA_DiachromaticMatrix(&mat, dacr, normalize));
}

inline KOLIBA_FLUT * KOLIBA_ConvertDiachromaticMatrixToFlut(KOLIBA_FLUT *fLut, const KOLIBA_DIACHROMA *dacr, unsigned int normalize) {
	KOLIBA_MATRIX mat;

	return KOLIBA_ConvertMatrixToFlut(fLut, KOLIBA_DiachromaticMatrix(&mat, dacr, normalize));
}

#endif

// You can apply any matrix in the YCbCr space. It is easy to convert RGB to
// YCbCr. Simply pass the RGB values defining the conversion of RGB to gray
// as defined by Rec. 2020 (or the other recs, or even your own) to create a
// conversion matrix.
//
// Remember, to modify a matrix A by a matrix B, A has to be the modificand
// and B the modifier in the call to KOLIBA_MultiplyMatrices().

KLBDC KOLIBA_MATRIX * KOLIBA_RgbToYcc(
	KOLIBA_MATRIX * output,
	const KOLIBA_RGB * const rec
);

// Then apply your desired matrix (i.e., modify the above matrix by your matrix),
// then modify that by an inverse conversion matrix.

KLBDC KOLIBA_MATRIX * KOLIBA_YccToRgb(
	KOLIBA_MATRIX * output,
	const KOLIBA_RGB * const rec
);

// All that said, you can just call KOLIBA_YccMatrix, pass to it your matrix
// and the desired Rec (or NULL for the default) and Koliba will figure it out
// for you and convert the matrix to modify a YcbCr version of an RGB pixel,
// without the need to convert the pixel to the YCbCr space.

KLBDC KOLIBA_MATRIX * KOLIBA_YccMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const input,
	const KOLIBA_RGB * const rec
);

// If you are working with a video footage shot to some older Rec, but you are
// trying to use it in whatever other Rec you need, just convert from one Rec
// (real or made up) to another.

KLBDC KOLIBA_MATRIX * KOLIBA_RecRec(
	KOLIBA_MATRIX * output,
	const KOLIBA_RGB * const from,
	const KOLIBA_RGB * const to
);

// And here are some of the real ones you can pass to KOLIBA_RecRec:

KLBDC extern const KOLIBA_RGB KOLIBA_Rec601;
KLBDC extern const KOLIBA_RGB KOLIBA_Rec709;
KLBDC extern const KOLIBA_RGB KOLIBA_Rec2020;
#define	KOLIBA_Rec2100	(KOLIBA_Rec2020)

// And some macros/inlines for converting among them.

#ifdef	NOKLINLIN

#define	KOLIBA_ConvertRgb601To709(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec601, &KOLIBA_Rec709))
#define	KOLIBA_ConvertRgb601To2020(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec601, &KOLIBA_Rec2020))
#define	KOLIBA_ConvertRgb709To2020(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec709, &KOLIBA_Rec2020))
#define	KOLIBA_ConvertRgb709To601(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec709, &KOLIBA_Rec601))
#define	KOLIBA_ConvertRgb2020To601(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec2020, &KOLIBA_Rec601))
#define	KOLIBA_ConvertRgb2020To709(mat)	(KOLIBA_RecRec(mat, &KOLIBA_Rec2020, &KOLIBA_Rec709))

#else

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb601To709(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec601, &KOLIBA_Rec709));
}

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb601To2020(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec601, &KOLIBA_Rec2020));
}

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb709To2020(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec709, &KOLIBA_Rec2020));
}

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb709To601(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec709, &KOLIBA_Rec601));
}

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb2020To601(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec2020, &KOLIBA_Rec601));
}

inline KOLIBA_MATRIX * KOLIBA_ConvertRgb2020To709(KOLIBA_MATRIX * matrix) {
	return (KOLIBA_RecRec(matrix, &KOLIBA_Rec2020, &KOLIBA_Rec709));
}

#endif

// Is there more? But of course!
//
// Apply four matrices, one (Y) to all channels, then three (R,G,B) to
// individual channels. These are usually RGB matrices, not YCbCr.

KLBDC KOLIBA_MATRIX * KOLIBA_TetraMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const Y,
	const KOLIBA_MATRIX * const R,
	const KOLIBA_MATRIX * const G,
	const KOLIBA_MATRIX * const B
);

// For example, we could have a CHROMAT Y, three CHROMAs R, G, B (all three
// using the model from Y), then we could something like this (and yes, this
// is just an example of its possibilities),

#ifdef	NOKLINLIN

#define	KOLIBA_TetraMat(output,y,r,g,b) do {\
	KOLIBA_MATRIX Y_123, R_456, G_789, B_0QW;\
	KOLIBA_TetraMatrix(\
		(output),\
		KOLIBA_ChromaticMatrix(&Y_123, (y)),\
		KOLIBA_ChromaMatrix(&R_456, r, &((y)->model)),\
		KOLIBA_ChromaMatrix(&G_789, g, &((y)->model)),\
		KOLIBA_ChromaMatrix(&B_0QW, b, &((y)->model))\
	);\
} while(0)

#else

inline KOLIBA_MATRIX * KOLIBA_TetraMat(KOLIBA_MATRIX *output, const KOLIBA_CHROMAT *y, const KOLIBA_CHROMA *r, const KOLIBA_CHROMA *g, const KOLIBA_CHROMA *b) {
	KOLIBA_MATRIX Y, R, G, B;
	return KOLIBA_TetraMatrix(
		output,
		KOLIBA_ChromaticMatrix(&Y, y),
		KOLIBA_ChromaMatrix(&R, r, &(y->model)),
		KOLIBA_ChromaMatrix(&G, g, &(y->model)),
		KOLIBA_ChromaMatrix(&B, b, &(y->model))
	);
}

#endif

// Make a grayscale matrix, red + green + blue must not equal 0.

KLBDC KOLIBA_MATRIX * KOLIBA_GrayscaleMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_RGB * const rec
);
#define	KOLIBA_GreyscaleMatrix(o,r)	KOLIBA_GrayscaleMatrix(o,r)

// Check if a 3x4 matrix really represents a 3x3 matrix. Returns true or
// false, or actually 1 and 0 since old-fashioned C knows nothing about
// booleans.

KLBDC unsigned int KOLIBA_MatrixIs3x3(
	const KOLIBA_MATRIX * const mat
);

// Make a 3x4 matrix represent a 3x3 matrix.

KLBDC KOLIBA_MATRIX * KOLIBA_MatrixTo3x3(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const mat
);

// Invert a matrix if possible.

KLBDC KOLIBA_MATRIX * KOLIBA_InvertMatrix(
	KOLIBA_MATRIX * output,
	const KOLIBA_MATRIX * const mat
);

// As powerful as matrices are, not all luttable effects can be reduced to a
// matrix. At least not to the 3x4 (with an implied 4th row) matrix as above.
//
// It was my Palette-Mallet OFX plug-in that led to the creation of the Koliba
// library. The idea behind the Palette was inspired by the homunculus in the
// movie Hugo. It made me think that if such a homunculus was programmed to
// paint rather than draw and was given a standard palette, then programmed to
// dip its brush in the eight colors of the palette and press the brush
// against paper, making predetermined strokes, it would create some painting.
//
// But if the automaton was given a palette of different colors, it would
// produce a painting that in some ways is the same as the one made with the
// standard palette, yet different in other ways.
//
// So, the Koliba palette given an image will not change that image if its
// palette is the standard black, white, red, green, blue, cyan, magenta, and
// yellow. But if we change any of the colors in the palette, the image will
// become repainted with the new palette, while keeping the same shapes as
// before.
//
// All the colors in the palette are expected to be limited to the {0-1} range
// in each channel, but each color also comes with its own efficacy, which can
// have any value. The value should probably be "within reason" but what that
// reason is is left to the Koliba library user.
//
// The palette also comes with a boolean flag for Erythropy, which was
// described earlier. Last but not least the entire palette has an overall
// efficacy, thus allowing fading the changes in and out, as well as
// exaggerating the effect and creating an opposite effect.
//
// We can check the validity of a palette (valid for both,
// KOLIBA_PALETTE and KOLIBA_PALETTE2 after a typecast),
// i.e., check all RGB values are within the [0..1] range.
//
// Return value of 0 (false) means the palette is not valid,
// otherwise it is valid.

KLBDC unsigned int KOLIBA_IsPaletteValid(
	const KOLIBA_PALETTE * const plt
);
// Apply a ring to a palette.
// Do not change red and black, or overall efficacy.

KLBDC KOLIBA_PALETTE * KOLIBA_ApplyPaletteRing(
	KOLIBA_PALETTE *output,
	const KOLIBA_PALETTE * const input,
	KOLIBA_Pluts plut
);

// And, as usual, we can convert a palette to a SLUT. We can then convert the
// SLUT to the corresponding FLUT as described earlier in this document.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertPaletteToSlut(
	KOLIBA_SLUT * output,
	const KOLIBA_PALETTE * const input
);

// These reset a palette or one of its pigments to the default.

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPalette(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteBlack(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteWhite(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteRed(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteGreen(
	KOLIBA_PALETTE * plt
);
KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteBlue(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteCyan(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteMagenta(
	KOLIBA_PALETTE * plt
);

KLBDC KOLIBA_PALETTE * KOLIBA_ResetPaletteYellow(
	KOLIBA_PALETTE *plt
);

// We can apply erythropy directly to a SLUT as well.
KLBDC KOLIBA_SLUT * KOLIBA_ApplyErythropy(
	KOLIBA_SLUT * output,
	const KOLIBA_SLUT * const input
);

// Apply pure erythropy to a FLUT. It is "pure" because the svit is at its
// defaults and has no effect on the result.
KLBDC KOLIBA_FLUT * KOLIBA_ApplyPureErythropy(KOLIBA_FLUT * output, const KOLIBA_VERTEX * const red);

// The other way to produce a SLUT from the beginning of Koliba development is
// the MALLET. Unlike with the palette, the mallet controls such things as
// contrast, saturation, and even the natural contrast, which is a concept
// similar to the classical contrast, except that it is different for every
// farba in the SLUT. That allows adjusting the contrast of an image in a new
// and much better way that has been done traditionally (IMHO).
//
// There are actually 19 fundamental mallets in Koliba, each affecting
// different farba (and svit) aspects. They are enumerated as "fundmal" near
// the start of this file.

// Initialize a mallet to its defaults.

KLBDC KOLIBA_MALLET * KOLIBA_InitializeMallet(
	KOLIBA_MALLET * mal,
	unsigned char flags
);

// Initialize all 19 fundamental mallets to their defaults.

KLBDC KOLIBA_MALLET * KOLIBA_InitializeFundamentalMallets(
	KOLIBA_MALLET * mal
);

// Convert an individual mallet to a SLUT. If the mallet is NULL or the flags
// equal 0, copy iLut to the output. If iLut is NULL, the identity SLUT is
// used. If the gray factors structure is NULL, Rec. 2020 is used.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertMalletToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_SLUT * const iLut,
	const KOLIBA_MALLET * const mallet,
	const KOLIBA_RGB * const gray
);

// Convert a number of mallets to a SLUT. iLut may be NULL, gray may be NULL.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertMalletsToSlut(
	KOLIBA_SLUT *sLut,
	const KOLIBA_SLUT * const iLut,
	const KOLIBA_MALLET * const mallets,
	const KOLIBA_RGB * const gray,
	unsigned int n
);

// Multiply a number of mallets into a sLut. iLut and gray may be NULL.

KLBDC KOLIBA_SLUT * KOLIBA_MultiplyMalletsToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_SLUT * const iLut,
	const KOLIBA_MALLET * const mallets,
	const KOLIBA_RGB * const gray,
	unsigned int n
);

// Convert a color filter to a SLUT. This is meant to emulate a color filter
// mounted on or in front of the camera objective ("lens"). For that to work,
// it should be applied as the first SLUT in a chain of SLUTs.

KLBDC KOLIBA_SLUT * KOLIBA_ConvertColorFilterToSlut(
	KOLIBA_SLUT * sLut,
	const KOLIBA_CFLT * const cFlt
);

// Apply a ring to a SLUT. Apply strut to the ring color.

KLBDC KOLIBA_SLUT * KOLIBA_ApplyStrutRing(KOLIBA_SLUT *sLut, const KOLIBA_VERTEX * const ring, KOLIBA_Pluts plut, double strut);




// Change the extent of the farba. This does not affect the svit, so you need
// to set (initialize) it either before or after calling this. Setting the
// range affects the softness of the colors, or can completely overturn them,
// changing the chrominance of the red, green, and blue channels independently
// of each other.
//
// N.B.: All vertices must be pointing at valid LUT data already. The function
// overwrites all the farba vertices (it does not change their addresses).

KLBDC KOLIBA_VERTICES * KOLIBA_FarbaRange(
	KOLIBA_VERTICES * const vert,
	const KOLIBA_RGB * const from,
	const KOLIBA_RGB * const to
);


// Sometimes a simple look-up table (SLUT) may seem, well, too simple because
// of its linear (ok, tri-linear) nature that applies the same effect in the
// way to a pixel with no regard for its brightness  of saturance.
//
// An obvious solution might be to build a large LUT. That increases the
// complexity of applying a perhaps otherwise simple effect. A better solution
// often is the creation of a mask that decides how strongly to apply the LUT
// based on some, perhaps non-linear, criteria.
//
// One way of obtaining that is to have two tracks with the same clip, keep
// the original on one of the tracks (typically the background of the two
// tracks), applying the LUT (or other effect) to the other track, and have
// the video editor combine the two tracks.
//
// Alas, not all editors have the ability to combine two tracks based on the
// brightness or the saturance, let alone a combination of both. That is why
// Koliba comes with Lumidux, which can do just that and it does not even need
// two tracks.

// We apply KOLIBA_LDX to a foreground and background pixels. In case of
// pixels modified by an effect, the modified pixel is the foreground and the
// original (before the effect) is the background.
//
// Note that the final argument can be NULL, and if so, Rec. 2020 will be used
// by default.

KLBDC KOLIBA_XYZ * KOLIBA_ApplyLumidux(
	KOLIBA_XYZ * output,					// output xyz/pixel
	const KOLIBA_XYZ * const foreground,	// foreground xyz/pixel
	const KOLIBA_XYZ * const background,	// background xyz/pixel
	const KOLIBA_LDX * const lumidux,		// Lumidux to apply
	const KOLIBA_RGB * const rec			// May be NULL
);

// More importantly, apply Lumidux to a XYZ after applying a FLUT. That is,
// apply the FLUT first, then call ApplyLumidux passing the original pixel as
// its background parameter and the result of having applied the LUT as the
// foreground parameter.

KLBDC KOLIBA_XYZ * KOLIBA_LumiduxXyz(
	KOLIBA_XYZ * output,			// output xyz/pixel
	const KOLIBA_XYZ * input,		// original xyz/pixel
	const KOLIBA_FLUT *fLut,		// FLUT to apply
	KOLIBA_FLAGS flags,				// FLUT flags to use
	const KOLIBA_LDX * lumidux,		// Lumidux to apply
	const KOLIBA_RGB * rec			// May be NULL
);


// Despite all of the above, sometimes, just sometimes, an effect cannot be
// expressed by a simple tri-linear look-up table, not even when used with a
// mask. In such cases, we need to work with complex LUTs. Even then it is
// important to create LUTs with just enough complexity to be able to affect
// the desired result without going overboard. The biggest question to ask
// yourself is will anyone other than you notice any difference between a huge
// and complex LUT on one hand, and a much simpler and smaller LUT on the
// other.



// Calculate the index of FLINDEX or FLBINDEX or FLWINDEX from the input x, y,
// z and the 3D FLUT dimensions. Also, scale and shift x, y, z accordingly.
// Optionally, write the i, j, k indices to the ind[3] array (which may be
// NULL if we do not want the indices.)

KLBDC signed int KOLIBA_GetFindex(
	KOLIBA_XYZ * xout,
	const KOLIBA_XYZ * const xin,
	const unsigned int dim[3],
	unsigned int ind[3]
);

// Create a FLUT from FLINDEX and a base.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertFlindexToFlut(
	KOLIBA_FLUT *fLut,
	const KOLIBA_FLINDEX * const flindex,
	const double * const base
);

// And the same from FLBINDEX and a base.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertFlbindexToFlut(
	KOLIBA_FLUT *fLut,
	const KOLIBA_FLBINDEX * const flbindex,
	const double * const base
);

// And from FLWINDEX and a base.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertFlwindexToFlut(
	KOLIBA_FLUT *fLut,
	const KOLIBA_FLWINDEX * const flwindex,
	const double * const base
);

// Apply Xyz based on a non-indexed fLut array, with a non-indexed flags
// array. It still is for larger LUTs, so we still calculate an index but it
// is an index to an array of KOLIBA_FLUTs and an array of proper flags. But
// we are not creating the FLUT from a base and a FLINDEX, nor are we looking
// up the flags in a condensed list.
//
// In other words, we use it when the above flags are KOLIBA_NOFLINDEX |
// KOLIBA_NOFFLINDEX.
//
// To be able to insert it in a function array, we have two extra arguments
// that we do not use.
//
// We are expecting xyzout and xyzin to be pointing at valid addresses, and
// are not asserting it because this function can easily be called millions
// of times in a row, and we prefer not to waste time.

KLBDC KOLIBA_XYZ * KOLIBA_NonindexedXyz(
	KOLIBA_XYZ *xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const fLut,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const,
	const void * const
);

// Apply Xyz based on a byte-indexed fLut array, with a non-indexed flags
// array.
//
// In other words, we use it when the above flags are KOLIBA_USEFLBINDEX |
// KOLIBA_NOFFLINDEX.
//
// To be able to insert it in a function array, we have one extra argument
// that we do not use.
//
// We are expecting xyzout and xyzin to be pointing at valid addresses, and
// are not asserting it because this function can easily be called millions
// of times in a row, and we prefer not to waste time.

KLBDC KOLIBA_XYZ * KOLIBA_ByteIndexedXyz(
	KOLIBA_XYZ *xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flbindex,
	const void * const
);

// Apply Xyz based on a 16-bit-indexed fLut array, with a non-indexed flags
// array.
//
// In other words, we use it when the above flags are KOLIBA_USEFLWINDEX |
// KOLIBA_NOFFLINDEX.
//
// To be able to insert it in a function array, we have one extra argument we
// do not use.
//
// We are expecting xyzout and xyzin to be pointing at valid addresses, and
// are not asserting it because this function can easily be called millions
// of times in a row, and we prefer not to waste time.

KLBDC KOLIBA_XYZ * KOLIBA_ShortIndexedXyz(
	KOLIBA_XYZ *xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flwindex,
	const void * const
);

// Apply Xyz based on a 32-bit-indexed fLut array, with a non-indexed flags
// array.
//
// In other words, we use it when the above flags are KOLIBA_USEFLINDEX |
// KOLIBA_NOFFLINDEX.
//
// To be able to insert it in a function array, we have one extra argument we
// do not use.
//
// We are expecting xyzout and xyzin to be pointing at valid addresses, and
// are not asserting it because this function can easily be called millions
// of times in a row, and we prefer not to waste time.

KLBDC KOLIBA_XYZ * KOLIBA_IndexedXyz(
	KOLIBA_XYZ *xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flindex,
	const void * const
);

///////////////////////////////////////////////////////////
//
// Same as above, but with KOLIBA_USEFFLBINDEX (8-bit flag indices).
//
KLBDC KOLIBA_XYZ * KOLIBA_NonindexedByteXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const fLut,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_ByteIndexedByteXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flbindex,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_ShortIndexedByteXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flwindex,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_IndexedByteXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flindex,
	const void * const findex
);
//
///////////////////////////////////////////////////////////
//
// Same, but with KOLIBA_USEFFLWINDEX (16-bit flag indices).
//
KLBDC KOLIBA_XYZ * KOLIBA_NonindexedShortXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const fLut,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_ByteIndexedShortXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flbindex,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_ShortIndexedShortXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flwindex,
	const void * const findex
);

KLBDC KOLIBA_XYZ * KOLIBA_IndexedShortXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flindex,
	const void * const findex
);
//
///////////////////////////////////////////////////////////
//
// Same but with KOLIBA_USEONEFFLAG (one flag for all).
//
KLBDC KOLIBA_XYZ * KOLIBA_NonindexedOneXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const fLut,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const,
	const void * const
);

KLBDC KOLIBA_XYZ * KOLIBA_ByteIndexedOneXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flbindex,
	const void * const
);

KLBDC KOLIBA_XYZ * KOLIBA_ShortIndexedOneXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flwindex,
	const void * const
);

KLBDC KOLIBA_XYZ * KOLIBA_IndexedOneXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	const double * const base,
	const KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	const void * const flindex,
	const void * const
);
//
///////////////////////////////////////////////////////////
//
// That is 16 separate functions. To be able to use them
// without having to remember which is which, we can use
// the right function pointer of the KOLIBA_INDEXEDXYZ type.
// Using your flag combination, you can do something like this:
//
// unsigned int myflags = KOLIBA_USEFLINDEX | KOLIBA_NOFFLINDEX;
// KOLIBA_INDEXEDXYZ MyXyzCall = KOLIBA_IndexedXyzCall(myflags);
// MyXyzCall(...);
//
// Or you could just call it directly from the array, like this:
//
// KOLIBA_IndexedXyzCalls[myflags](...);
//
// But in that case, make sure myflags < 16 (which it is if you
// have assigned to it a combination of the two index flags as
// described earlier in this section).
//
// Unfortunately, gcc does not seem to know that void * can be
// used for any pointer type, so we have to declared all 16
// functions as using void * for the last two parameters, and
// then we have create separate pointers of the correct type
// in the code. Gee thanks, gcc for bloating our code!!! :(
//
// Meanwhile Microsoft C compiler knows the standard, so it is
// not issuing these pointless warnings. Alas, gcc is in wide
// use, which has forced me to make this portion of my code less
// efficient just to avoid the improper warnings from gcc.
//
// And of course because of this, it is not clear from this .h
// file what kind of variables these functions expect for the
// void * parameters. :(

KLBDC extern const KOLIBA_INDEXEDXYZ KOLIBA_IndexedXyzCalls[16];

#ifdef	NOKLINLIN

#define	KOLIBA_IndexedXyzCall(x)	KOLIBA_IndexedXyzCalls[x&15]

#else

inline KOLIBA_INDEXEDXYZ KOLIBA_IndexedXyzCall(unsigned char index) {
	return KOLIBA_IndexedXyzCalls[index & 15];
}

#endif


// Convert between any number of pixels and vertices. That is, convert
// between a 32-bit pixel and a 64-bit rgb/xyz vertex.

KLBDC KOLIBA_PIXEL * KOLIBA_VertexToPixel(
	KOLIBA_PIXEL * p,
	const KOLIBA_VERTEX * const v,
	unsigned int n
);

KLBDC KOLIBA_VERTEX * KOLIBA_PixelToVertex(
	KOLIBA_VERTEX * v,
	const KOLIBA_PIXEL * const p,
	unsigned int n
);

// Compare whether the values in the vertex are the same as those in the pixel
// converted to doubles.

KLBDC int KOLIBA_PixelIsVertex(
	const KOLIBA_VERTEX * const v,
	const KOLIBA_PIXEL * const p
);

// Check if a text string could be a valid .cube title. Returns 0 if it is,
// 1 if it contains an invalid character, -1 if it is too long.

KLBDC signed int KOLIBA_CheckCubeTitle(
	const char * const title
);

// Set the vertices of a cube-style simple LUT to point at the corresponding
// vertices in a SLUT.

KLBDC KOLIBA_CUBE_VERTICES * KOLIBA_CubeVerticesFromSlut(
	KOLIBA_CUBE_VERTICES * v,
	const KOLIBA_SLUT * const sLut
);

// For creating a .cube compatible series of KOLIBA_RGB values
// you can make your own function like this:
// This is almost identical to typedef (*KOLIBA_EXTERNAL)(), except it supports a separate
// input and output RGB structures.
//
// For example, it could look something like this:
//
// KOLIBA_RGB * MakeMyVertex(KOLIBA_RGB *rgbout, KOLIBA_RGB *rgbin, const void *params) {
//	double r, g, b;
//	double halfpi = *(double *)params;
//
//	r = rgbin->r; g = rgbin->g; b = rgbin->b;
//
//	rgbout->r = sin(halfpi*(r + g*g + b*b*b) / 3.0);
//	rgbout->g = sqrt(1.0-cos(halfpi*(r*r*r + g + b*b) / 3.0));
//	rgbout->b = (r*r + g*g*g + b) / 3.0;
//
//	return rgbout;
//}
//
// That is actually a very nice effect, which is best done using a LUT
// because doing all that math for every pixel would take a while. But
// we only need to create a LUT once and save it in a file for repeat use.
//
// We would need to know the value of pi/2.0, which = asin(1.0). We would
// store it in a global or static variable, so we do not have to compute every
// time the function is called. Or we can just use KOLIBA_PiDiv2...
//
// This is the above sample (*KOLIBA_MAKEVERTEX)() function for testing.
// Remember, it expects (double *)&KOLIBA_PiDiv2 as its params.

KLBDC KOLIBA_RGB * KOLIBA_MakeVertex(
	KOLIBA_RGB * rgbout,
	const KOLIBA_RGB * const rgbin,
	const void * const params
);

// The "pure color" effect. Good for showing compression flaws in a photo or
// video. And for producing pure color (i.e., one with 100% saturation, except
// when the input is a shade of gray, which shall be left unchanged).
//
// Its "params" can either be NULL or be a KOLIBA_RGB structure with red,
// green and blue impurities.

KLBDC KOLIBA_RGB * KOLIBA_PureColor(
	KOLIBA_RGB * rgbout,
	const KOLIBA_RGB * const rgbin,
	const void * const params
);

// The "color mask" KOLIBA_MAKEVERTEX effect. It produces a grayscale mask of
// the "green screen" type, though you can pick any one of the three channels,
// and you have the option of basing it on the negative of each pixel. You can
// adjust the contrast of the result. You can base it on the hue or the
// chrominance, and these can be absolute or relative.
//
// The purpose of it all is to experiment with various settings and to learn
// how the various options affect the resultant mask. To accomplish all this,
// the "params" has to be a structure with all these options.
//
// The speed member can be between {0-6}, same as the two speeds in the
// KOLIBA_LDX structure. The channel byte has to be 0 for red, 1 for green, or
// 2 for blue.
//
// The low member must not be greater than the high member, though they may be
// equal.
//
// The flags may be combined. They are defined here. The negative flags
// inverts the input pixel before processing it. The invert flag inverts the
// output pixel after processing.
#define	KOLIBA_COLORMASK_NEGATIVE	1
#define	KOLIBA_COLORMASK_CHROMINANCE	2
#define	KOLIBA_COLORMASK_RELATIVE	4
#define KOLIBA_COLORMASK_INVERT		8
typedef struct _KOLIBA_COLORMASK {
	double low;
	double high;
	unsigned char speed;
	unsigned char channel;
	unsigned char flags;
} KOLIBA_COLORMASK;

// Make a series of RGB values in the same order as used in the .cube files.
// Though this code is more general than what cubes do, so we can have a
// different dimension for each axis. Let m be the dimension of the red (or X)
// axis, n the dimension of the green (or Y) axis, and o the dimension of the
// blue (or Z) axis.
//
// The caller must pass us a pointer to (m+1)*(n+1)*(o+1) KOLIBA_RGB
// structures, the values of m, n, o, and a pointer to a
// (*KOLIBA_MAKEVERTEX)() function and its parameters. We will call the
// function (m+1)(n+1)(o+1) times, each time passing to it a different input
// RGB structure.
//
// Please note that to produce values suitable to create a .cube file of p*p*p
// dimensions, you have to pass m = p-1, n = p-1, o = p-1, because cube files
// pass the number of needed vertices, not the number of cubes to make.
//
// The values of each dimension must be no less than 1 and no more than 256.
// To make a valid .cube file, the values must be no more than 255 and all
// three have to be the same.

KLBDC KOLIBA_RGB * KOLIBA_MakeCube(
	KOLIBA_RGB * cube,
	unsigned int m,
	unsigned int n,
	unsigned int o,
	KOLIBA_MAKEVERTEX fn,
	const void * const params
);
// Make a series of RGB identity values in the same order as used in the .cube
// files.

KLBDC KOLIBA_RGB * KOLIBA_MakeIdentityCube(
	KOLIBA_RGB * cube,
	unsigned int m,
	unsigned int n,
	unsigned int o
);

// Create a FLUT from a cube. The cube is an array of (m+1)(n+1)(o+1)
// KOLIBA_RGBs. We are given m, n, o, as well as i, j, k, where i is the red
// from m, j is the green from n, and k is the blue from o.
//
// And while the FLUT is dimensioned [m][n][o], the cube is dimensioned
// [o][n][m] for some reasons.
//
// The m, n, o dimensions are passed as an array of three integers. The i, j,
// k indices are passed as another array of three integers.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertCubeToFlut(
	KOLIBA_FLUT * fLut,
	const KOLIBA_RGB * const cube,
	const unsigned int dim[3],
	const unsigned int ind[3]
);

// Create m*n*o FLUTs from a cuboid.

KLBDC KOLIBA_FLUT * KOLIBA_ConvertCubeToFluts(
	KOLIBA_FLUT * fLuts,
	const KOLIBA_RGB * const cube,
	const unsigned int dim[3]
);

// Given a (*KOLIBA_MAKEVERTEX)() function, create a partial SLUT, i.e., the
// sLut[i][j][k] portion of a large SLUT.

KLBDC KOLIBA_SLUT * KOLIBA_MakePartialSlut(
	KOLIBA_SLUT * sLut,
	const unsigned int dim[3],
	const unsigned int ind[3],
	KOLIBA_MAKEVERTEX fn,
	const void * const params
);

// Apply Xyz based on an on-the-fly fLut array. That is, the array is filled
// as we go. We determine whether a specific fLut has to be filled by
// comparing its flags to 0. Both, the fLut array and the flag array must be
// in a writable section of the memory.
//
// Hint: If you need to change any parameters to the (*KOLIBA_MAKEVERTEX)()
// function, just zero-out the entire flag array to re-apply the function with
// its new parameters.

KLBDC KOLIBA_XYZ * KOLIBA_FlyXyz(
	KOLIBA_XYZ * xyzout,
	const KOLIBA_XYZ * const xyzin,
	KOLIBA_FLUT * fLut,
	KOLIBA_FLAGS * const flags,
	const unsigned int dim[3],
	KOLIBA_MAKEVERTEX fn,
	const void * const params
);

// Convert a tint to a FLUT, either directly or via a SLUT, based on function
// arguments.
//
// While we are using a KOLIBA_VERTEX structure with its r, g, b members for
// the tint, these should be presented in the user interface as cyan, magenta,
// and yellow.

KLBDC KOLIBA_FLUT * KOLIBA_TintToFlut(
	KOLIBA_FLUT * f,
	const KOLIBA_VERTEX * const tint,
	double saturation,
	double tinge,
	double light,
	char invert,
	char flut,
	const KOLIBA_RGB * const rec
);

// Convert between a linear double and sRGB. https://en.wikipedia.org/wiki/SRGB

KLBDC double KOLIBA_LinearToSrgb(
	double linear
);
KLBDC double KOLIBA_SrgbToLinear(
	double srgb
);

// Convert between a count of linear doubles and sRGB.

KLBDC double * KOLIBA_DoublesToSrgb(
	double * outp,
	const double * const inp,
	unsigned int count
);

KLBDC double * KOLIBA_SrgbToDoubles(
	double * outp,
	const double * const inp,
	unsigned int count
);

// Convert between 8bpp RGBA and sRGB.

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8ToSrgb(
	KOLIBA_RGBA8PIXEL * pixelout,
	const KOLIBA_RGBA8PIXEL * const pixelin,
	unsigned int count
);

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_SrgbToRgba8(
	KOLIBA_RGBA8PIXEL * pixelout,
	const KOLIBA_RGBA8PIXEL * const pixelin,
	unsigned int count
);

// Convert XYZ to the RGB portion of RGBA8PIXEL. Since this is likely to be
// called millions of times, we are not wasting time checking for NULL
// pointers, and leave that responsibility up to the caller.
//
// oconv, however, may be NULL.
//
// NB: We do not set px->a to anything, so the caller can set it as needed,
// before or after calling us.

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_XyzToRgba8Pixel(
	KOLIBA_RGBA8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

// Convert scaled XYZ to the RGB portion of RGBA8PIXEL.
// Same as KOLIBA_XyzToRgba8Pixel() but the result is
// already premultiplied by 255.0 because the FLUT was scaled.
KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_ScaledXyzToRgba8Pixel(
	KOLIBA_RGBA8PIXEL *px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

// Convert the RGB portion of RGBA8PIXEL to XYZ.

KLBDC KOLIBA_XYZ * KOLIBA_Rgba8PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_RGBA8PIXEL * const px,
	const double * const iconv
);

// The same for BGRA8.

KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_XyzToBgra8Pixel(
	KOLIBA_BGRA8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_ScaledXyzToBgra8Pixel(
	KOLIBA_BGRA8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Bgra8PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_BGRA8PIXEL * const px,
	const double * const iconv
);

// ARGB.

KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_XyzToArgb8Pixel(
	KOLIBA_ARGB8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_ScaledXyzToArgb8Pixel(
	KOLIBA_ARGB8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Argb8PixelToXyz(
	KOLIBA_XYZ *xyz,
	const KOLIBA_ARGB8PIXEL * const px,
	const double * const iconv
);

// ABGR.

KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_XyzToAbgr8Pixel(
	KOLIBA_ABGR8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_ScaledXyzToAbgr8Pixel(
	KOLIBA_ABGR8PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	const unsigned char * const oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Abgr8PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_ABGR8PIXEL * const px,
	const double * const iconv
);

// While we use small look-up tables for the conversion between 8-bit integers
// and 64-bit doubles, doing so for larger types would require a lot of memory.
//
// So, the library first converts any other type to 64 bits, then applies any
// optional routine you may pass to it in order to modify it further. In the
// opposite direction, the library applies any optional routine you pass to it
// to modify the 64-bit double float values, and then converts the result to
// the target type.

KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_XyzToRgba32Pixel(
	KOLIBA_RGBA32PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	KOLIBA_DBLCONV oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Rgba32PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_RGBA32PIXEL * const px,
	KOLIBA_DBLCONV iconv
);

KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_XyzToBgra32Pixel(
	KOLIBA_BGRA32PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	KOLIBA_DBLCONV oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Bgra32PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_BGRA32PIXEL * const px,
	KOLIBA_DBLCONV iconv
);

KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_XyzToArgb32Pixel(
	KOLIBA_ARGB32PIXEL * px,
	const KOLIBA_XYZ * const xyz,
	KOLIBA_DBLCONV oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Argb32PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_ARGB32PIXEL * const px,
	KOLIBA_DBLCONV iconv
);

KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_XyzToAbgr32Pixel(
	KOLIBA_ABGR32PIXEL *px,
	const KOLIBA_XYZ * const xyz,
	KOLIBA_DBLCONV oconv
);

KLBDC KOLIBA_XYZ * KOLIBA_Abgr32PixelToXyz(
	KOLIBA_XYZ * xyz,
	const KOLIBA_ABGR32PIXEL * const px,
	KOLIBA_DBLCONV iconv
);

// Video editors usually use the 32-bit floating point for video data, which
// comprises of various view transforms, such as ACES of all sorts, with new
// transforms appering regularly. Koliba allows you to "plug in" your own
// transform from the non-linear 32-bit view to the linear 64-bit view needed
// in Koliba and another transform back from the linear 64-bit view to
// whatever non-linear 32-bit view transform your work requires.
//
// The transforms need to be C-style functions of the following format:
// On success, the functions must return the same output pointer they
// were called with. On failure, they must return NULL.
// Any other return values are deemed invalid and may cause unpredictable
// results in the functions that call these transforms.
//
// There is no need to create your own linear-to-linear transforms as
// Koliba can convert between 32-bit and 64-bit floats internally.
// Just pass NULL for Koliba to use the internal conversion.

KLBDC KOLIBA_PIXEL * KOLIBA_ApplyPixel(
	KOLIBA_PIXEL *pxout,
	const KOLIBA_PIXEL *pxin,
	const KOLIBA_FLUT *fLut,
	KOLIBA_FLAGS flags,
	KOLIBAPIXELTOXYZ transformin,
	KOLIBAXYZTOPIXEL transformout
);

KLBDC KOLIBA_PIXEL * KOLIBA_PolyPixel(
	KOLIBA_PIXEL *pxout,
	const KOLIBA_PIXEL *pxin,
	const KOLIBA_FFLUT *ffLut,
	unsigned int n,
	KOLIBAPIXELTOXYZ transformin,
	KOLIBAXYZTOPIXEL transformout
);

KLBDC KOLIBA_PIXEL * KOLIBA_ExternalPixel(
	KOLIBA_PIXEL *pxout,
	const KOLIBA_PIXEL *pxin,
	const KOLIBA_FFLUT *ffLut,
	unsigned int n,
	unsigned int m,
	KOLIBA_EXTERNAL ext,
	void *params,
	KOLIBAPIXELTOXYZ transformin,
	KOLIBAXYZTOPIXEL transformout
);


// It is common  to use a quick approximation of video
// effects while editing (typically called a preview mode).
// In that case, these functions may come useful.

// Convert KOLIBA_SLUT to a palette of 256 KOLIBA_RGBA8PIXEL
// values This can be used, among other things, for a low-quality
// but fast preview during editing. The alpha channels of all
// palette members are set to fully opaque (i.e., 255).

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_SlutToRgba8Palette(
	KOLIBA_RGBA8PIXEL *palette,
	KOLIBA_SLUT * const sLut,
	const double * const iconv,
	const unsigned char * const oconv
);

// Apply a 256-member RGBA8 palette to a number
// of RGBA8 pixels discarding their Alpha channel.

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_PaletteToRgba8(
	KOLIBA_RGBA8PIXEL * outframe,
	const KOLIBA_RGBA8PIXEL * const inframe,
	const KOLIBA_RGBA8PIXEL * const palette,
	unsigned int count
);

// Apply a 256-member RGBA8 palette to a number
// of RGBA8 pixels preserving their Alpha channel.

KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_PaletteToRgba8Alpha(
	KOLIBA_RGBA8PIXEL * outframe,
	const KOLIBA_RGBA8PIXEL * const inframe,
	const KOLIBA_RGBA8PIXEL * const palette,
	unsigned int count
);





// Convert doubles from MSB first to whatever byte order
// the local system uses (FixDoubles) or from the local
// system to MSB firt (NetDoubles). Either of these should
// only be called once on the same data, otherwise the
// result will not be what you might expect, and it may be
// different on different systems (but it is system-independent
// if only called once).
//
// The d parameter is a pointer to one or more doubles;
// the n parameter is the number of doubles to convert.

#ifdef	BIGENDIAN

#define	KOLIBA_FixDoubles(d,n)	(d)
#define	KOLIBA_NetDoubles(d,n)	(d)

#else

KLBDC double * KOLIBA_FixDoubles(
	double *d,
	unsigned int n
);

KLBDC double * KOLIBA_NetDoubles(
	double *d,
	unsigned int n
);

#endif

// Check the sum of the data in a file.

KLBDC int KOLIBA_CheckSum(
	const double * const d,
	double chsum,
	unsigned int n
);

// Calculate the checksum of the data to save in a file.

KLBDC double KOLIBA_CalcSum(
	const double * const d,
	unsigned int n
);






// This gets the library version, in the ((major << 16) | minor) format.

KLBDC unsigned int KOLIBA_GetLibraryVersion(void);

KLBDC unsigned int KOLIBA_GetMajorLibraryVersion(void);

KLBDC unsigned int KOLIBA_GetMinorLibraryVersion(void);




/****************************************************************************/
/****************                                           *****************/
/**************** T H E  KOLIBA  S T R I N G  F O R M A T S *****************/
/****************                                           *****************/
/****************************************************************************/

// Sometimes we may want to convert a Koliba data structure into a text
// string, perhaps to post it in an online forum or pass the data through
// an ASCII-text-only communication channel.
//
// We want to do that without converting binary reals into plain text,
// and then back to binary (we would only need to do that if we wanted
// the values to be human readable). Thankfully, the C printf and scanf
// routines make it very easy to print and scan real values as hexadecinal
// text.

// Get the data type of a Koliba data string.
// The string mut be at least SLTCFILEHEADERBYTES
// bytes in size.

KLBDC KOLIBA_ftype KOLIBA_GetStringDataFormat(const unsigned char * const data);

// The KOLIBA_SLUT structure can be converted to a string with these:
KLBDC extern const char KOLIBA_PrintSlttFormat[];
#define pfmt	KOLIBA_PrintSlttFormat
// We can then convert the text back to a KOLIBA_SLUT structure by using
// the following scan-format string:
KLBDC extern const char KOLIBA_ScanSlttFormat[];
#define	sfmt	KOLIBA_ScanSlttFormat
// And just to check the header,
KLBDC extern const char KOLIBA_ScanSlttHeaderFormat[];
// We can use built-in functions to do that conversion for us. Note that
// the string size must be at least SLTAMINCHARS chars.
KLBDC char * KOLIBA_SlutToString(
	char * string,
	const KOLIBA_SLUT * const sLut,
	unsigned int strsize
);

KLBDC KOLIBA_SLUT * KOLIBA_StringToSlut(
	KOLIBA_SLUT * sLut,
	const char * const string
);

// Similarly, we can convert a KOLIBA_MATRIX to text.
KLBDC extern const char KOLIBA_PrintM34tFormat[];
KLBDC extern const char KOLIBA_ScanM34tFormat[];
KLBDC extern const char KOLIBA_ScanM34tHeaderFormat[];

KLBDC char * KOLIBA_MatrixToString(
	char * string,
	const KOLIBA_MATRIX * const m3x4,
	unsigned int strsize
);

KLBDC KOLIBA_MATRIX * KOLIBA_StringToMatrix(
	KOLIBA_MATRIX * m3x4,
	const char * const string
);

// We also have a text version (*.chrt) of the chromat file.
KLBDC extern const char KOLIBA_PrintChrtFormat[];
KLBDC extern const char KOLIBA_ScanChrtFormat[];
KLBDC extern const char KOLIBA_ScanChrtHeaderFormat[];

KLBDC char * KOLIBA_ChromatToString(
	char * string,
	const KOLIBA_CHROMAT * const chrm,
	unsigned int strsize
);

KLBDC KOLIBA_CHROMAT * KOLIBA_StringToChromat(
	KOLIBA_CHROMAT * chrm,
	const char * const string
);

// Text version of Koliba palette (.pltt)
KLBDC extern const char KOLIBA_PrintPlttFormat[];
KLBDC extern const char KOLIBA_ScanPlttFormat[];
KLBDC extern const char KOLIBA_ScanPlttHeaderFormat[];





/****************************************************************************/
/******************                                       *******************/
/****************** T H E  KOLIBA  F I L E  F O R M A T S *******************/
/******************                                       *******************/
/****************************************************************************/

// We can export the effects we are working on to files, so we can either
// share the effects with the people we are working with, or perhaps with
// everyone. Or we can save them in the file at end of the day, so we can load
// them back the next day and continue working on them. Or we can save stages
// of our work as we progress, so we can go back if we decide something did
// not turn out the way we wanted.
//
// We have defined a number of binary file formats for that. With the addition
// of the Koliba string formats (see the previous section), we can also save
// those same structures in a text format. We may want to do that especially
// when we want to write down some comment on what functions and what arguments
// we used to create the file. This is possible because any Koliba functions
// used to read such text files stop reading after they have receved the entire
// structure, and just ignore anything written after the text representing
// the structures.

// Get the data type of a Koliba data file header.

KLBDC KOLIBA_ftype KOLIBA_GetFileDataFormat(const unsigned char * const header);


// The SaLUTe (Simple Look-up Table) file (extension .sLut) is the format to
// save the final look-up table for some effect, so we can use it from now on,
// or sell or give it to other. The file contains exactly one data stream with
// the following structure:
//
// The first 16 bytes contain the UUID 549fb300-9c06-11e8-9d31-003048fd9f6e
// in the big-endian binary format. This is immediately followed by a 64-bit
// size, also in the big-endian format. The size is always 8*24+8 = 200. It
// does not include the header or itself (so, only the data that follow the
// size).
//
// Next, 24 doubles containing the values of the 8 rows of 3 columns each, of
// the double cube LUT, i.e., the KOLIBA_SLUT data type.
//
// After that, an 8-byte checksum, calculated from the 24 doubles treated as
// an array with indices i = 0, 1, 2, ..., 23. The checksum starts at 0.0,
// then with each index we add (double)(i+1)*value[i]. In other words, it is
// a sum [i=1..24](value[i]).
//
// Please, be advised that the order of the colors in the table is  different
// from that used in the .cube files. The order is:
//
//		Black
//		Blue
//		Green
//		Cyan
//		Red
//		Magenta
//		Yellow
//		White
//
// Each of the above consists of the three channels  in the red, green, and
// blue order.
//
// In other words, black.r, black.g, black.b, blue.r, ... white.blue. The
// values represent a simple tri-linear LUT, i.e., NOT tetrahedral.
//
// All that can be expressed by the KOLIBA_SLUT and KOLIBA_SLUT2 data types.
//
// The .sLut file then always starts with the header,

KLBDC extern const unsigned char KOLIBA_sLutHeader[SLTCFILEHEADERBYTES];

// After reading and verifying its header, we can get the SLUT from the file,
// convert the values from MSB first to the local system (using
// KOLIBA_FixDoubles() declared earlier in this header), and check if it is a
// valid SLUT (returns 1 if valid, 0 if not, so it really is Boolean, but
// since C does not have a Boolean type, we use int).

#ifdef	NOKLINLIN

#define KOLIBA_CheckSlut(sLut,chsum)	KOLIBA_CheckSum((double*)(sLut),(double)(chsum),(unsigned int)(sizeof(KOLIBA_SLUT)/sizeof(double)))

#else

inline int KOLIBA_CheckSlut(KOLIBA_SLUT *sLut, double chsum) {
	return KOLIBA_CheckSum((double *)sLut, chsum, sizeof(KOLIBA_SLUT)/sizeof(double));
}

#endif


// Write a SLUT to a named file. It creates the file first. It closes the file
// at the end. Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteSlutToNamedFile(
	const KOLIBA_SLUT *sLut,
	const char *fname
);

// Read a SLUT from a named file. Returns sLut on success, NULL on failure.
// If, however, sLut is not NULL, its contents will be filled with the
// identity SLUT on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromNamedFile(
	KOLIBA_SLUT *sLut,
	char *fname
);

// Sometimes we may need to communicate a sLut over a text-only communications
// channel, or save it in a plain-text-format file. To do so, we can convert
// the KOLIBA_SLUT structure into text using the following format string:
//
// Such a file should have the .sltt extension and MUST start with the
// text formated by KOLIBA_PrintSlttFormat[]. The text MAY be followed by any other text,
// which we will ignore. But no comments are permissable within the
// KOLIBA_PrintSlttFormat-formated string, or else the KOLIBA_ScanSlttFormat[] scan would fail.

// Write a SLUT to a named .sltt file. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WriteSlttToNamedFile(
	const KOLIBA_SLUT *sLut,
	const char *fname
);

// Read a Lut from a named .sltt file. Returns Lut on success,
// NULL on failure. If, however, Lut is not NULL, its
// contents will be filled with the identity Lut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlttFromNamedFile(
	KOLIBA_SLUT *Lut,
	char *fname
);


// If an effect can be expressed as a 3x4 matrix (i.e., KOLIBA_MATRIX), it
// should be saved in the 3x4 Matrix file (extension .m3x4), rather than the
// .fLut file because it is very easy to convert a matrix into not just a SLUT
// but also directly to a FLUT. This file contains exactly one data stream
// with the following structure:
//
// The first 16 bytes contain the UUID fb8abc9c-c357-11e8-9d79-003048fd9f6e
// in the big-endian binary format. This is immediately followed by a 64-bit
// size, also in the big-endian format. The size is always 8*12+8 = 104. It
// does not include the header or itself (so, only the data that follow the
// size).
//
// Next, 12 doubles of the 3x4 matrix, which are actually the first three rows
// of a 4x4 matrix, in which the 4th row is always 0 0 0 1, and, therefore, is
// implied and need not be stored in the file. Each row represents the red,
// green, blue coefficients and an offset. An identity matrix would look like
// this:
//
//		1 0 0 0
//		0 1 0 0
//		0 0 1 0
//
// with the implied fourth row,
//
//		0 0 0 1
//
// After that, an 8-byte checksum, calculated from the 12 doubles treated as
// an array with indices i = 0, 1, 2, ..., 11. The checksum starts at 0.0,
// then with each index we add (double)(i+1)*value[i]. In other words, it is a
// sum [i=1..12](value[i]).
//
// We can express the matrix as the KOLIBA_MATRIX and KOLIBA_MATRIX2 data
// types.
//
// The .m3x4 file then always starts with the following,

KLBDC extern const unsigned char KOLIBA_m3x4Header[SLTCFILEHEADERBYTES];

// After reading and verifying its header, we can get the matrix from the file
// and convert it to a SLUT or a FLUT, as needed or desired.

#ifdef	NOKLINLIN

#define KOLIBA_CheckMat(m,c)	KOLIBA_CheckSum((double*)(m),(double)(c),(unsigned int)(sizeof(KOLIBA_MATRIX)/sizeof(double)))

#else

inline int KOLIBA_CheckMat(KOLIBA_MATRIX *matrix, double chsum) {
	return KOLIBA_CheckSum((double*)matrix, chsum, sizeof(KOLIBA_MATRIX)/sizeof(double));
}

#endif

// Write a MATRIX to a named file. Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteMatrixToNamedFile(
	const KOLIBA_MATRIX *mat,
	const char *fname
);

// Read a MATRIX from a named file. Returns m3x4 on success, NULL on failure.
// If, however, m3x4 is not NULL, its contents will be filled with the
// identity MATRIX on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromNamedFile(
	KOLIBA_MATRIX *m3x4,
	char *fname
);

// As before, we can store the same data in a text format.

KLBDC int KOLIBA_WriteM34tToNamedFile(
	const KOLIBA_MATRIX *m3x4,
	const char *fname
);

KLBDC KOLIBA_MATRIX * KOLIBA_ReadM34tFromNamedFile(
	KOLIBA_MATRIX *m3x4,
	char *fname
);

// If we want to save a palette in a file, we can convert it to a simple LUT
// and save it as a .sLut file.
//
// But if we want to keep the ability to edit the palette in the future, we
// can save it in a .kPlt file.
//
// For that, we use the slightly different KOLIBA_PALETTE2 structure (mostly
// because we like to use our checksum algorithm).

// So what happens to erythropy? How do we save it in a file?
//
// Simple! We use a different UUID for a palette that has its erythropy set to
// 0 (so effectively it does not use erythropy) and another one for a palette
// that has erythropy set to 1 (or any other non-zero value, i.e., true).
//
// The file with no erythropy shall have the UUID of
// 7de56100-5a59-11e9-9df9-003048fd9f6e, while the one with erythropy set to
// true shall use 7de56101-5a59-11e9-9df9-003048fd9f6e for its UUID. As
// always, these are MSB first and followed with the length of the
// KOLIBA_PALETTE2 data (which is 8*4*8+8+8=272), again MSB first, and then
// with the KOLIBA_PALETTE2 data MSB first.
//
// In either case, the file extension shall be .kPlt (case sensitive). Their
// respective headers are,

KLBDC extern const unsigned char KOLIBA_kPltHeader00[SLTCFILEHEADERBYTES];
KLBDC extern const unsigned char KOLIBA_kPltHeader01[SLTCFILEHEADERBYTES];

// And their checksum macro/inline,

#ifdef	NOKLINLIN

#define KOLIBA_CheckKplt(kPlt,chsum)	KOLIBA_CheckSum((double*)(kPlt),(double)(chsum),(unsigned int)((sizeof(KOLIBA_PALETTE2)/sizeof(double))-1))

#else

inline int KOLIBA_CheckKplt(KOLIBA_PALETTE2 * kPlt, double chsum) {
	return KOLIBA_CheckSum((double*)kPlt, chsum, (sizeof(KOLIBA_PALETTE2)/sizeof(double))-1);
}

#endif

// Write a kPlt to a named file. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WritePaletteToNamedFile(
	const KOLIBA_PALETTE *kPlt,
	const char *fname
);

// Read a palette from a named file. Returns palette on success,
// NULL on failure. If, however, kPlt is not NULL, its
// contents will be filled with the identity palette on failure.

KLBDC KOLIBA_PALETTE * KOLIBA_ReadPaletteFromNamedFile(
	KOLIBA_PALETTE *kPlt,
	char *fname
);



// A chromat file (*.chrm) starts with the 16-byte UUID of
// c4dc1f47-5713-11e9-9df5-003048fd9f6e in the big-endian format, followed by
// an 8-byte size of the rest of the file (72), again in the big-endian
// format, followed by a KOLIBA_CHROMAT structure in the big-endian format,
// followed by the checksum of said structure, in the big-endian format.

#ifdef	NOKLINLIN

#define KOLIBA_CheckChrm(chrm,chsum)	KOLIBA_CheckSum((double*)(chrm),(double)(chsum),(unsigned int)(sizeof(KOLIBA_CHROMAT)/sizeof(double)))

#else

inline int KOLIBA_CheckChrm(KOLIBA_CHROMAT *chrm, double chsum) {
	return KOLIBA_CheckSum((double*)chrm, chsum, sizeof(KOLIBA_CHROMAT)/sizeof(double));
}

#endif

KLBDC extern const unsigned char KOLIBA_chrmHeader[SLTCFILEHEADERBYTES];

// Write a CHROMATIC MATRIX to a named file. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WriteChromaticMatrixToNamedFile(
	const KOLIBA_CHROMAT *chrm,
	const char *fname
);

// Read a CHROMATIC MATRIX from a named file. Returns chrm on success, NULL on
// failure. If, however, chrm is not NULL, its contents will be filled with
// the identity CHROMATIC MATRIX, using the Rec. 2020 model, on failure.

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChromaticMatrixFromNamedFile(
	KOLIBA_CHROMAT *chrm,
	char *fname
);

// Write a KOLIBA_CHROMAT to a named .chrt file.
// Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteChrtToNamedFile(
	const KOLIBA_CHROMAT *chrt,
	const char *fname
);

// And read the text version from a named file.

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChrtFromNamedFile(
	KOLIBA_CHROMAT *chrt,
	char *fname
);

// The dichroma file (*.dicr) starts with the 16-byte UUID of
// ax58598e-6de9-11e9-9e0b-003048fd9f6e in the big-endian format. The x in the
// first byte stores some flags described below. As usual, this follows by an
// eight-byte big-endian size of the rest of the file, which consists of a
// KOLIBA_DICHROMA structure, followed by a checksum.

// The flag nybble of the first header byte contains the following
// information:
//
// The high bit (bit 3) determines whether the dichroma is to be normalized
// (yes if set, no otherwise).
//
// Bits 0-1 contain a value between 0 and 2 (value of 3 is not valid) to
// determine which RGB channel to use in the math:
//
//		0 = red
//		1 = green
//		2 = blue
//

#ifdef	NOKLINLIN

#define KOLIBA_CheckDicr(dicr,chsum)	KOLIBA_CheckSum((double*)(dicr),(double)(chsum),(unsigned int)(sizeof(KOLIBA_DICHROMA)/sizeof(double)))

#else

inline int KOLIBA_CheckDicr(KOLIBA_DICHROMA * dicr, double chsum) {
	return KOLIBA_CheckSum((double*)dicr, chsum, sizeof(KOLIBA_DICHROMA)/sizeof(double));
}

#endif

// Because the low nybble of the first byte of a dicr header is not always the
// same, this particular header has it set to 0, so any routine reading or
// writing the header needs to deal with that.

KLBDC extern const unsigned char KOLIBA_dicrHeader[SLTCFILEHEADERBYTES];

// Write a dichromatic matrix to a named file.
// Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteDichromaticMatrixToNamedFile(
	const KOLIBA_DICHROMA *dicr,
	const char *fname,
	unsigned int normalize,	// true or false
	unsigned int channel	// MUST be < 3
);

// Read a dichromatic matrix from a named file. Returns the
// matrix on success, NULL on failure. If, however, dicr is
// not NULL, its contents will be filled with the identity
// dichromatic matrix, using the Rec. 2020 model, on failure.

KLBDC KOLIBA_DICHROMA * KOLIBA_ReadDichromaticMatrixFromNamedFile(
	KOLIBA_DICHROMA *dicr,
	char *fname,
	unsigned int *normalize,	// May be NULL
	unsigned int *channel		// May be NULL
);


// The Color Filter file (extension .cFlt) contains exactly one data stream
// with the following structure:
//
// The first 16 bytes contain the UUID c180ec6a-c66a-11e8-9d82-003048fd9f6e in
// the big-endian binary format. This is immediately followed by a 64-bit
// size, also in the big-endian format. The size is always 4*8+8 = 40.
//
// Next, 3 doubles of the filter color, and one double of filter density.
// After that, an 8-byte checksum, calculated from the 4 doubles treated as an
// array with indices i = 0, 1, 2, 3. The checksum starts at 0.0, then with
// each index we add (double)(i+1)*value[i]. In other words, it is a
// sum [i=1..4](value[i]).
//
// We can express the filter as the KOLIBA_CFLT and KOLIBA_CFLT2 structures.

// The .cFlt file then always starts with the header,

KLBDC extern const unsigned char KOLIBA_cFltHeader[SLTCFILEHEADERBYTES];

// After reading and verifying its header, we can get the color
// filter from the file and convert it to a SLUT.

#ifdef	NOKLINLIN

#define KOLIBA_CheckCflt(cFlt,chsum)	KOLIBA_CheckSum((double*)(cFlt),(double)(chsum),(unsigned int)(sizeof(KOLIBA_CFLT)/sizeof(double)))

#else

inline int KOLIBA_CheckCflt(KOLIBA_CFLT *cFlt, double chsum) {
	return KOLIBA_CheckSum((double*)cFlt, chsum, sizeof(KOLIBA_CFLT)/sizeof(double));
}

#endif

// Write a COLOR FILTER to a named file. Returns 0 on success, non-0 on
// failure.

KLBDC int KOLIBA_WriteColorFilterToNamedFile(
	const KOLIBA_CFLT *cFlt,
	const char *fname
);

// Read a Color Filter from a named file. Returns Color Filter on success,
// NULL on failure. If, however, cFlt is not NULL, its
// contents will be filled with zeros on failure.

KLBDC KOLIBA_CFLT * KOLIBA_ReadColorFilterFromNamedFile(
	KOLIBA_CFLT *cFlt,
	char *fname
);

// Read a sLut from a named compatible file. Returns sLut on success,
// NULL on failure. If, however, sLut is not NULL, its
// contents will be filled with the identity sLut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromCompatibleNamedFile(
	KOLIBA_SLUT *sLut,
	char *fname,
	KOLIBA_ftype *ft
);

// Read a matrix from a named compatible file. Returns mat on success,
// NULL on failure. If, however, mat is not NULL, its
// contents will be filled with the identity mat on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromCompatibleNamedFile(KOLIBA_MATRIX *mat, char *fname, KOLIBA_ftype *ft);


// A Lumidux file, extension .ldx (lower case only) is different from all
// other file formats presented here in that it does not represent an effect
// but a MASK to control the strength of an effect on individual pixels based
// on their luminance and/or chrominance (or optionally, saturation). The file
// has the following structure:
//
// It starts with the 698489f0-b4b0-11e8-9d62-003048fd9f6e uuid, in MSB first
// binary. Indeed, everything in the file is MSB first (i.e., network order).
//
// The uuid is followed by a 64-bit integer (MSB first) which contains the
// size of the remaining data (so, not including the uuid or the size itself).
//
// The next 64 bits (8 bytes) contain the the data defined by the KOLIBA_LDX
// structure. It is described in detail earlier in this document.
//
// That is 5 64-bit values altogether, or 5*8 = 40 bytes, so the length will
// contain a 64-bit MSB-first integer = 40. Along with the uuid and the length
// integer, the entire file is 64 bytes long. There is no checksum.
//
// If both, luminance and saturance, are off, Lumidux has no effect, i.e., the
// efficacy of the foreground pixel is 1. When used with effects (e.g., with
// SLUTs, matrices, and such), the pixel processed by the effect is the
// foreground, while the original pixel (before the effect was applied) is the
// background.
//
// When neither luminance nor saturance is off, their combined effect is the
// geometric mean of their respective effects, i.e., the square root of the
// product of the two effects is the final efficacy of the foreground over the
// background.
//
// When one of them is off and the other is not, only the effect of the one
// that is not off is applied as the efficacy.
//
// Additionally, if and only if at least one of them is not off and the mask
// flag is set, all three channels of the return pixel are set to the
// efficacy, so that a grayscale mask is returned. This is not likely to be
// the case when Lumidux is loaded from a file, though if it is, then the user
// should have the option to turn the mask flag off, even if perhaps no other
// options from the file are changeable (e.g., when converting a SLUT and such
// from a binary file to a .cube text file), or it may just be turned off by
// default in a simple conversion utility.
//
// N.B. The low threshold must be less or equal to the corresponding high
//		threshold. If it is not, the file is considered invalid or corrupted
//		and must be rejected.

KLBDC extern const unsigned char KOLIBA_ldxHeader[SLTCFILEHEADERBYTES];


















/****************************************************************************/
/**************************                       ***************************/
/************************** L O C A L   S T U F F ***************************/
/**************************                       ***************************/
/****************************************************************************/



// This is local to the library. It is not exported, and it is not guaranteed
// to be present in any other version of the library.

#ifndef SWIG
// Copy RGB color to all vertices of a sLut.
KLBHID KOLIBA_SLUT * KOLIBA_CopyColorToSlutVertices(KOLIBA_SLUT *sLut, const KOLIBA_RGB *rgb);



// Some locally useful data.
KLBHID extern const KOLIBA_SLUT KOLIBA_Ones;
KLBHID extern const KOLIBA_SLUT KOLIBA_NaturalContrastSlut;
KLBHID extern const KOLIBA_SLUT KOLIBA_Rec2020Slut;
KLBHID extern const double KOLIBA_NaN;
#endif

// And some globally useful.
KLBDC extern const KOLIBA_SLUT KOLIBA_NaturalFarbaContrastSlut;




/****************************************************************************/
/******************                                       *******************/
/****************** KOLIBA  I N L I N E S  &  M A C R O S *******************/
/******************                                       *******************/
/****************************************************************************/

// Here are some useful inlines. Some of them were functions but were removed
// because using them all would bloat the library unnecessarily. Others were
// never functions for the same reason: They would increase the size of the
// library and probably just waste memory.
//
// If your compiler is old enough not to support C99, it may not understand
// inlines. In that case, the -Dinline=__inline command line option may work
// (if it supports C90). If that does not work either, -DNOKLINLIN will turn
// off the inline functions and replace them with oldfashioned macros, which
// all C compilers should support.
//
// Each macro is preceded by a "function prototype" or at least what the
// function prototype would be if it was an actual function. This is to help
// you see what types of parameters each macro expects.
//
// Please remember not to do something like:
//
//		KOLIBA_Rgba8Pixel(output++, input++, ...);
//
// Using ++ with a macro will not behave in the way you may be hoping for!
// Though using it with the inlines will.
//
// N.B.	Neither the inlines nor the macros copy the alpha channel from the
//		input to the output. This is to allow their use with software that
//		does not support alpha channels.
//
//		There is, however, a nifty way to copy the alpha channel from the
//		input to the output if we remember that these functions (but not
//		macros) return the output type, so appending ->a = pixelin->a to the
//		function will copy the input to the output (this assumes pixelin is
//		the name of the input, you will of course use whatever name you use).
//		Like this:
//
//			KOLIBA_Rgba8Pixel(pixelout, pixelin, etc...)->a = pixelin->a;
//
//		And it works will all these inlines, not justKOLIBA_Rgba8Pixel.

#ifndef	NOKLINLIN

inline KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_ScaledRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToRgba8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_PolyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

// N.B. Only the last FLUT in the chain needs to be scaled,
// all others must not be scaled (unless you want some special result
// I have not thought of). This is true about the other 8-bit
// pixel poly macros, too.
inline KOLIBA_RGBA8PIXEL * KOLIBA_ScaledPolyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToRgba8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_ExternalRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_FlyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_LumiduxRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_RGBA8PIXEL * KOLIBA_IndexedRgba8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba8Pixel(pixelout, idfn(&xyz, KOLIBA_Rgba8PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

// Unlike the previous functions in this section, this one has nothing to do
// with LUTs but allows us to combine two input pixels (foreground and
// background) into a single output pixel based on luminance or saturance or
// both.
//
// Do not confuse it with the KOLIBA_LumiduxRgba8Pixel function declared
// above.

inline KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8PixelLumidux(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *foreground, const KOLIBA_RGBA8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Rgba8PixelToXyz(&fore, foreground, iconv), KOLIBA_Rgba8PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//

inline KOLIBA_BGRA8PIXEL * KOLIBA_Bgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_ScaledBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToBgra8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_PolyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_ScaledPolyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToBgra8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_ExternalBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_FlyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_LumiduxBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_IndexedBgra8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra8Pixel(pixelout, idfn(&xyz, KOLIBA_Bgra8PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_BGRA8PIXEL * KOLIBA_Bgra8PixelLumidux(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *foreground, const KOLIBA_BGRA8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Bgra8PixelToXyz(&fore, foreground, iconv), KOLIBA_Bgra8PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//

inline KOLIBA_ARGB8PIXEL * KOLIBA_Argb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_ScaledArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToArgb8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_PolyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_ScaledPolyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToArgb8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_ExternalArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_FlyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_LumiduxArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_IndexedArgb8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb8Pixel(pixelout, idfn(&xyz, KOLIBA_Argb8PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_ARGB8PIXEL * KOLIBA_Argb8PixelLumidux(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *foreground, const KOLIBA_ARGB8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Argb8PixelToXyz(&fore, foreground, iconv), KOLIBA_Argb8PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//


inline KOLIBA_ABGR8PIXEL * KOLIBA_Abgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}
inline KOLIBA_ABGR8PIXEL * KOLIBA_ScaledAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToAbgr8Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_PolyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_ScaledPolyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_ScaledXyzToAbgr8Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_ExternalAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_FlyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_LumiduxAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_IndexedAbgr8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, idfn(&xyz, KOLIBA_Abgr8PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_ABGR8PIXEL * KOLIBA_Abgr8PixelLumidux(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *foreground, const KOLIBA_ABGR8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Abgr8PixelToXyz(&fore, foreground, iconv), KOLIBA_Abgr8PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

///////////////

inline KOLIBA_RGBA32PIXEL * KOLIBA_Rgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_PolyRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_ExternalRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_FlyRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_LumiduxRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_IndexedRgba32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToRgba32Pixel(pixelout, idfn(&xyz, KOLIBA_Rgba32PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_RGBA32PIXEL * KOLIBA_Rgba32PixelLumidux(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *foreground, const KOLIBA_RGBA32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Rgba32PixelToXyz(&fore, foreground, iconv), KOLIBA_Rgba32PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//

inline KOLIBA_BGRA32PIXEL * KOLIBA_Bgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_PolyBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_ExternalBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_FlyBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_LumiduxBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_IndexedBgra32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToBgra32Pixel(pixelout, idfn(&xyz, KOLIBA_Bgra32PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_BGRA32PIXEL * KOLIBA_Bgra32PixelLumidux(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *foreground, const KOLIBA_BGRA32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Bgra32PixelToXyz(&fore, foreground, iconv), KOLIBA_Bgra32PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//

inline KOLIBA_ARGB32PIXEL * KOLIBA_Argb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_PolyArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_ExternalArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_FlyArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_LumiduxArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_IndexedArgb32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToArgb32Pixel(pixelout, idfn(&xyz, KOLIBA_Argb32PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_ARGB32PIXEL * KOLIBA_Argb32PixelLumidux(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *foreground, const KOLIBA_ARGB32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Argb32PixelToXyz(&fore, foreground, iconv), KOLIBA_Argb32PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

//

inline KOLIBA_ABGR32PIXEL * KOLIBA_Abgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_ApplyXyz(&xyz, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), fLut, flags), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_PolyAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_PolyXyz(&xyz, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), ffLut, n), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_ExternalAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_ExternalXyz(&xyz, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), ffLut, n, m, ext, params), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_FlyAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_FlyXyz(&xyz, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, dim, fn, params), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_LumiduxAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, zyx;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_LumiduxXyz(&zyx, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), fLut, flags, lumidux, rec), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_IndexedAbgr32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL * const pixelin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, idfn(&xyz, KOLIBA_Abgr32PixelToXyz(&xyz, pixelin, iconv), base, flags, dim, flindex, findex), oconv);
}

inline KOLIBA_ABGR32PIXEL * KOLIBA_Abgr32PixelLumidux(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *foreground, const KOLIBA_ABGR32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv) {
	KOLIBA_XYZ xyz, fore, back;
	return KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_ApplyLumidux(&xyz, KOLIBA_Abgr32PixelToXyz(&fore, foreground, iconv), KOLIBA_Abgr32PixelToXyz(&back, background, iconv), lumidux, rec), oconv);
}

#else	// defined(NOKLINLIN)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Rgba8Pixel(pxout,pxin,fLut,flags,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToRgba8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_ScaledRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledRgba8Pixel(pxout,pxin,fLut,flags,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_111;\
	KOLIBA_ScaledXyzToRgba8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_111, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_5_0_111, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_PolyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_PolyRgba8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToRgba8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_ScaledPolyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledPolyRgba8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_222;\
	KOLIBA_ScaledXyzToRgba8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_222, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_4_d_222, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_ExternalRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ExternalRgba8Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToRgba8Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_FlyRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv);

#define KOLIBA_FlyRgba8Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToRgba8Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_LumiduxRgba8Pixel(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxRgba8Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToRgba8Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// Rather than creating 16 RGBA8PIXEL macros that differ only by a function
// name, here is one macro to which we pass the right function, whether by
// name or as one of the array of such functions.
//
// KLBDC KOLIBA_RGBA8PIXEL * KOLIBA_IndexedRgba8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_RGBA8PIXEL *pxout, const KOLIBA_RGBA8PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_IndexedRgba8Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToRgba8Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Rgba8PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KOLIBA_RGBA8PIXEL * KOLIBA_Rgba8PixelLumidux(KOLIBA_RGBA8PIXEL *pixelout, const KOLIBA_RGBA8PIXEL *foreground, const KOLIBA_RGBA8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);
#define	KOLIBA_Rgba8PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_89_O, x_y_z_fore_17_65, x_y_z_back_39_P;\
	KOLIBA_XyzToRgba8Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_89_O, KOLIBA_Rgba8PixelToXyz(&x_y_z_fore_17_65, foreground, iconv), KOLIBA_Rgba8PixelToXyz(&x_y_z_back_39_P, background, iconv), lumidux, rec), oconv);\
} while(0)

//

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_Bgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Bgra8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToBgra8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_ScaledBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledBgra8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1111;\
	KOLIBA_ScaledXyzToBgra8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1111, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_5_0_1111, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_PolyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_PolyBgra8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToBgra8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_ScaledPolyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledPolyBgra8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_232;\
	KOLIBA_ScaledXyzToBgra8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_232, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_4_d_232, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_ExternalBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ExternalBgra8Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToBgra8Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_FlyBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv);

#define KOLIBA_FlyBgra8Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToBgra8Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_LumiduxBgra8Pixel(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxBgra8Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToBgra8Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA8PIXEL * KOLIBA_IndexedBgra8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_BGRA8PIXEL *pxout, const KOLIBA_BGRA8PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_IndexedBgra8Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToBgra8Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Bgra8PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KOLIBA_BGRA8PIXEL * KOLIBA_Bgra8PixelLumidux(KOLIBA_BGRA8PIXEL *pixelout, const KOLIBA_BGRA8PIXEL *foreground, const KOLIBA_BGRA8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);
#define	KOLIBA_Bgra8PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_89_O, x_y_z_fore_17_65, x_y_z_back_39_P;\
	KOLIBA_XyzToBgra8Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_89_O, KOLIBA_Bgra8PixelToXyz(&x_y_z_fore_17_65, foreground, iconv), KOLIBA_Bgra8PixelToXyz(&x_y_z_back_39_P, background, iconv), lumidux, rec), oconv);\
} while(0)

//

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_Argb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Argb8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToArgb8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_ScaledArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledArgb8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_171;\
	KOLIBA_ScaledXyzToArgb8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_171, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_5_0_171, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_PolyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_PolyArgb8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToArgb8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_ScaledPolyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledPolyArgb8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_21;\
	KOLIBA_ScaledXyzToArgb8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_21, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_4_d_21, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_ExternalArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ExternalArgb8Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToArgb8Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_FlyArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv);

#define KOLIBA_FlyArgb8Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToArgb8Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_LumiduxArgb8Pixel(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxArgb8Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToArgb8Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB8PIXEL * KOLIBA_IndexedArgb8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ARGB8PIXEL *pxout, const KOLIBA_ARGB8PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_IndexedArgb8Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToArgb8Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Argb8PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KOLIBA_ARGB8PIXEL * KOLIBA_Argb8PixelLumidux(KOLIBA_ARGB8PIXEL *pixelout, const KOLIBA_ARGB8PIXEL *foreground, const KOLIBA_ARGB8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Argb8PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_89_O, x_y_z_fore_17_65, x_y_z_back_39_P;\
	KOLIBA_XyzToArgb8Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_89_O, KOLIBA_Argb8PixelToXyz(&x_y_z_fore_17_65, foreground, iconv), KOLIBA_Argb8PixelToXyz(&x_y_z_back_39_P, background, iconv), lumidux, rec), oconv);\
} while(0)


//

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_Abgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Abgr8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToAbgr8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_ScaledAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledAbgr8Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_13;\
	KOLIBA_ScaledXyzToAbgr8Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_13, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_5_0_13, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_PolyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_PolyAbgr8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToAbgr8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_ScaledPolyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ScaledPolyAbgr8Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_29;\
	KOLIBA_ScaledXyzToAbgr8Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_29, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_4_d_29, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_ExternalAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_ExternalAbgr8Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToAbgr8Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_FlyAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, const double *iconv, const unsigned char *oconv);

#define KOLIBA_FlyAbgr8Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToAbgr8Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_LumiduxAbgr8Pixel(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxAbgr8Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToAbgr8Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR8PIXEL * KOLIBA_IndexedAbgr8Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ABGR8PIXEL *pxout, const KOLIBA_ABGR8PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_IndexedAbgr8Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToAbgr8Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Abgr8PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KOLIBA_ABGR8PIXEL * KOLIBA_Abgr8PixelLumidux(KOLIBA_ABGR8PIXEL *pixelout, const KOLIBA_ABGR8PIXEL *foreground, const KOLIBA_ABGR8PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define	KOLIBA_Abgr8PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_89_O, x_y_z_fore_17_65, x_y_z_back_39_P;\
	KOLIBA_XyzToAbgr8Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_89_O, KOLIBA_Abgr8PixelToXyz(&x_y_z_fore_17_65, foreground, iconv), KOLIBA_Abgr8PixelToXyz(&x_y_z_back_39_P, background, iconv), lumidux, rec), oconv);\
} while(0)


///////////////////

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_Rgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Rgba32Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToRgba32Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_PolyRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_PolyRgba32Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToRgba32Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_ExternalRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_ExternalRgba32Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToRgba32Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_FlyRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define KOLIBA_FlyRgba32Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToRgba32Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_LumiduxRgba32Pixel(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxRgba32Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToRgba32Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_IndexedRgba32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_RGBA32PIXEL *pxout, const KOLIBA_RGBA32PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_IndexedRgba32Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToRgba32Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Rgba32PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KLBDC KOLIBA_RGBA32PIXEL * KOLIBA_Rgba32PixelLumidux(KOLIBA_RGBA32PIXEL *pixelout, const KOLIBA_RGBA32PIXEL *foreground, const KOLIBA_RGBA32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Rgba32PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_32_007_JB, x_Y_z_fore_cl, x_y_Z_back_1965;\
	KOLIBA_XyzToRgba32Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_32_007_JB, KOLIBA_Rgba32PixelToXyz(&x_Y_z_fore_cl, foreground, iconv), KOLIBA_Rgba32PixelToXyz(&x_y_Z_back_1965, background, iconv), lumidux, rec), oconv);\
} while(0)

//

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_Bgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Bgra32Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToBgra32Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_PolyBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_PolyBgra32Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToBgra32Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_ExternalBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_ExternalBgra32Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToBgra32Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_FlyBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define KOLIBA_FlyBgra32Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToBgra32Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_LumiduxBgra32Pixel(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxBgra32Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToBgra32Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_IndexedBgra32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_BGRA32PIXEL *pxout, const KOLIBA_BGRA32PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_IndexedBgra32Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToBgra32Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Bgra32PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KLBDC KOLIBA_BGRA32PIXEL * KOLIBA_Bgra32PixelLumidux(KOLIBA_BGRA32PIXEL *pixelout, const KOLIBA_BGRA32PIXEL *foreground, const KOLIBA_BGRA32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Bgra32PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_32_007_JB, x_Y_z_fore_cl, x_y_Z_back_1965;\
	KOLIBA_XyzToBgra32Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_32_007_JB, KOLIBA_Bgra32PixelToXyz(&x_Y_z_fore_cl, foreground, iconv), KOLIBA_Bgra32PixelToXyz(&x_y_Z_back_1965, background, iconv), lumidux, rec), oconv);\
} while(0)

//

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_Argb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Argb32Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToArgb32Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_PolyArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_PolyArgb32Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToArgb32Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_ExternalArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_ExternalArgb32Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToArgb32Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_FlyArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define KOLIBA_FlyArgb32Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToArgb32Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_LumiduxArgb32Pixel(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxArgb32Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToArgb32Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_IndexedArgb32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ARGB32PIXEL *pxout, const KOLIBA_ARGB32PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_IndexedArgb32Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToArgb32Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Argb32PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KLBDC KOLIBA_ARGB32PIXEL * KOLIBA_Argb32PixelLumidux(KOLIBA_ARGB32PIXEL *pixelout, const KOLIBA_ARGB32PIXEL *foreground, const KOLIBA_ARGB32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Argb32PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_32_007_JB, x_Y_z_fore_cl, x_y_Z_back_1965;\
	KOLIBA_XyzToArgb32Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_32_007_JB, KOLIBA_Argb32PixelToXyz(&x_Y_z_fore_cl, foreground, iconv), KOLIBA_Argb32PixelToXyz(&x_y_Z_back_1965, background, iconv), lumidux, rec), oconv);\
} while(0)


//

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_Abgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Abgr32Pixel(pxout,pxin,fLut,flags,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_5_0_1;\
	KOLIBA_XyzToAbgr32Pixel(pxout, KOLIBA_ApplyXyz(&koliba_x_y_z_5_0_1, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_5_0_1, pxin, iconv), fLut, flags), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_PolyAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_PolyAbgr32Pixel(pxout,pxin,ffLut,n,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_4_d_2;\
	KOLIBA_XyzToAbgr32Pixel(pxout, KOLIBA_PolyXyz(&koliba_x_y_z_4_d_2, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_4_d_2, pxin, iconv), ffLut, n), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_ExternalAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FFLUT *ffLut, unsigned int n, unsigned int m, KOLIBA_EXTERNAL ext, void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_ExternalAbgr32Pixel(pxout,pxin,ffLut,n,m,ext,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_9_Q_17;\
	KOLIBA_XyzToAbgr32Pixel(pxout, KOLIBA_ExternalXyz(&koliba_x_y_z_9_Q_17, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_9_Q_17, pxin, iconv), ffLut, n, m, ext, params), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_FlyAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, KOLIBA_FLUT *fLut, KOLIBA_FLAGS *flags, const unsigned int dim[3], KOLIBA_MAKEVERTEX fn, const void *params, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define KOLIBA_FlyAbgr32Pixel(pxout,pxin,fLut,flags,dim,fn,params,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_13_J_0;\
	KOLIBA_XyzToAbgr32Pixel(pxout, KOLIBA_FlyXyz(&koliba_x_y_z_13_J_0, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_13_J_0, pxin, iconv), fLut, flags, dim, fn, params), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_LumiduxAbgr32Pixel(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *pixelin, const KOLIBA_FLUT *fLut, KOLIBA_FLAGS flags, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, const double *iconv, const unsigned char *oconv);

#define KOLIBA_LumiduxAbgr32Pixel(pxout,pxin,fLut,flags,ldx,rec,iconv,oconv) do {\
	KOLIBA_XYZ koliba_x_y_z_67_LD_4, LD_009_z_y_x;\
	KOLIBA_XyzToAbgr32Pixel(pxout, KOLIBA_LumiduxXyz(&LD_009_z_y_x, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_67_LD_4, pxin, iconv), fLut, flags, ldx, rec), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_IndexedAbgr32Pixel(KOLIBA_INDEXEDXYZ idfn, KOLIBA_ABGR32PIXEL *pxout, const KOLIBA_ABGR32PIXEL * const pxin, const double * const base, const unsigned int * const flags, const unsigned int dim[3], const void * const flindex, const void * const findex, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_IndexedAbgr32Pixel(idfn,pxout,pxin,base,flags,dim,flindex,findex,iconv,oconv)	do {\
	KOLIBA_XYZ koliba_x_y_z_4_Ind_31;\
	KOLIBA_XyzToAbgr32Pixel(pxout, idfn(&koliba_x_y_z_4_Ind_31, KOLIBA_Abgr32PixelToXyz(&koliba_x_y_z_4_Ind_31, pxin, iconv), base, flags, dim, flindex, findex), oconv);\
} while(0)

// KLBDC KOLIBA_ABGR32PIXEL * KOLIBA_Abgr32PixelLumidux(KOLIBA_ABGR32PIXEL *pixelout, const KOLIBA_ABGR32PIXEL *foreground, const KOLIBA_ABGR32PIXEL *background, const KOLIBA_LDX *lumidux, const KOLIBA_RGB *rec, KOLIBA_DBLCONV iconv, KOLIBA_DBLCONV oconv);

#define	KOLIBA_Abgr32PixelLumidux(pixelout,foreground,background,lumidux,rec,iconv,oconv) do {\
	KOLIBA_XYZ x_y_z_32_007_JB, x_Y_z_fore_cl, x_y_Z_back_1965;\
	KOLIBA_XyzToAbgr32Pixel(pixelout, KOLIBA_ApplyLumidux(&x_y_z_32_007_JB, KOLIBA_Abgr32PixelToXyz(&x_Y_z_fore_cl, foreground, iconv), KOLIBA_Abgr32PixelToXyz(&x_y_Z_back_1965, background, iconv), lumidux, rec), oconv);\
} while(0)




#endif	// NOKLINLIN


/****************************************************************************/
/**************                                              ****************/
/************** T H E   KOLIBA   F I L E   F U N C T I O N S ****************/
/**************                                              ****************/
/****************************************************************************/

// Here are some high level functions for reading and writing Koliba files. It
// is C library specific, which may or may not be desirable under various
// systems. If you want it, #define USECLIB before including koliba.h.
//
// Each function in this section is either for reading from/writing to a file
// that has already been open using the C library fopen function. Doing the
// same but to a named file has been defined in the file format section.
//
// The functions with "OpenFile" in their name simply do their work using the
// FILE pointer passed to them, and return. They do not close the file, the
// caller is expected to do so.
//
// The functions with "NamedFile" in their name will open (or create) the file
// and, if successful, will call the corresponding "OpenFile" functions, then
// close the file. The caller need not (cannot, actually) close the file.
//
// Even though you need to #define USECLIB to use these, they are not macros,
// nor are they inline functions. They are regular C functions included in the
// Koliba library. But the OPEN files receive their FILE * pointers, which had
// to be opened by the same C library as the Koliba library is compiled with.
// That is generally not a problem under Unix and its children. But it can cause
// havoc under Windows. Hence the need to #define USECLIB, so you have to pause
// before you act.

#ifdef USECLIB

#include <stdio.h>

// Write a SLUT to an open .sLut file. It needs to be open for writing binary
// data. It remains open upon return, so the caller needs to close it. Returns
// 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteSlutToOpenFile(
	const KOLIBA_SLUT *sLut,
	FILE *f
);

// Write a SLUT to an open .sltt file. It remains open upon
// return, so the caller needs to close it. Returns 0 on
// success, non-0 on failure.

KLBDC int KOLIBA_WriteSlttToOpenFile(const KOLIBA_SLUT *sLut, FILE *f);

// Write a MATRIX to an open .m3x4 file. It needs to be open for writing
// binary data. It remains open upon return, so the caller needs to close it.
// Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteMatrixToOpenFile(
	const KOLIBA_MATRIX *mat,
	FILE *f
);

KLBDC int KOLIBA_WriteM34tToOpenFile(
	const KOLIBA_MATRIX *m3x4,
	FILE *f
);

// Write a palette to an open .kPlt file. It needs to be open
// for writing binary data. It remains open upon return, so
// the caller needs to close it. Returns 0 on success, non-0
// on failure.

KLBDC int KOLIBA_WritePaletteToOpenFile(
	const KOLIBA_PALETTE *kPlt,
	 FILE *f
);

// Write a CHROMATIC MATRIX to an open .chrm file. It needs to be open for
// writing binary data. It remains open upon return, so the caller needs to
// close it. Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteChromaticMatrixToOpenFile(
	const KOLIBA_CHROMAT *chrm,
	FILE *f
);

// Write a KOLIBA_CHROMAT to an open .chrt file. It remains open
// upon return, so the caller needs to close it. Returns 0 on
// success, non-0 on failure.

KLBDC int KOLIBA_WriteChrtToOpenFile(
	const KOLIBA_CHROMAT *chrt,
	FILE *f
);

// Write a dichromatic matrix to an open .dicr file. It needs
// to be open for writing binary data. It remains open upon
// return, so the caller needs to close it. Returns 0 on
// success, non-0 on failure.

KLBDC int KOLIBA_WriteDichromaticMatrixToOpenFile(
	const KOLIBA_DICHROMA *dicr,
	FILE *f,
	unsigned int normalize,	// true or false
	unsigned int channel	// MUST be < 3
);

// Write a COLOR FILTER to an open .cFlt file. It needs to be open for writing
// binary data. It remains open upon return, so the caller needs to close it.
// Returns 0 on success, non-0 on failure.

KLBDC int KOLIBA_WriteColorFilterToOpenFile(
	const KOLIBA_CFLT *cFlt,
	FILE *f
);

// Read a SLUT from an open .sLut file. It needs to be open for reading binary
// data. It remains open upon return, so the caller needs to close it. Returns
// sLut on success, NULL on failure. If, however, sLut is not NULL, its
// contents will be filled with the identity SLUT on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromOpenFile(
	KOLIBA_SLUT *sLut,
	FILE *f
);

// Read a SLUT from an open .sltt file. It may to be open
// for reading binary or text data. It remains open upon return,
// so the caller needs to close it. Returns sLut on success, NULL
// on failure. If, however, sLut is not NULL, its contents
// will be filled with the identity sLut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlttFromOpenFile(
	KOLIBA_SLUT *sLut,
	FILE *f
);

// Read a MATRIX from an open .m3x4 file. It needs to be open for reading
// binary data. It remains open upon return, so the caller needs to close it.
// Returns m3x4 on success, NULL on failure. If, however, m3x4 is not NULL,
// its contents will be filled with the identity MATRIX on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromOpenFile(
	KOLIBA_MATRIX *m3x4,
	FILE *f
);

KLBDC KOLIBA_MATRIX * KOLIBA_ReadM34tFromOpenFile(
	KOLIBA_MATRIX *m3x4,
	FILE *f
);

// Read a palette from an open .kPlt file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns palette on success, NULL
// on failure. If, however, kPlt is not NULL, its contents
// will be filled with the identity palette on failure.

KLBDC KOLIBA_PALETTE * KOLIBA_ReadPaletteFromOpenFile(KOLIBA_PALETTE *kPlt, FILE *f);

// Read a CHROMATIC MATRIX from an open .chrm file. It needs to be open for
// reading binary data. It remains open upon return, so the caller needs to
// close it. Returns chrm on success, NULL on failure. If, however, chrm is
// not NULL, its contents will be filled with the identity CHROMATIC MATRIX,
// using the Rec. 2020 model, on failure.

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChromaticMatrixFromOpenFile(
	KOLIBA_CHROMAT *chrm,
	FILE *f
);

KLBDC KOLIBA_CHROMAT * KOLIBA_ReadChrtFromOpenFile(
	KOLIBA_CHROMAT *chrm,
	FILE *f
);

// Read a dichromatic matrix from an open .dicr file. It needs
// to be open for reading binary data. It remains open upon
// return, so the caller needs to close it. Returns dichromatic
// matrix on success, NULL on failure. If, however, dicr is
// not NULL, its contents will be filled with the identity
// dichromatic matrix, using the Rec. 2020 model, on failure.

KLBDC KOLIBA_DICHROMA * KOLIBA_ReadDichromaticMatrixFromOpenFile(
	KOLIBA_DICHROMA *dicr,
	FILE *f,
	unsigned int *normalize,	// May be NULL
	unsigned int *channel		// May be NULL
);


// Read a Color Filter from an open .cflt file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns matrix on success, NULL
// on failure. If, however, m3x4 is not NULL, its contents
// will be filled with zeros on failure.

KLBDC KOLIBA_CFLT * KOLIBA_ReadColorFilterFromOpenFile(
	KOLIBA_CFLT *cFlt,
	FILE *f
);

// Read a sLut from an open compatible file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns sLut on success, NULL
// on failure. If, however, sLut is not NULL, its contents
// will be filled with the identity sLut on failure.

KLBDC KOLIBA_SLUT * KOLIBA_ReadSlutFromCompatibleOpenFile(
	KOLIBA_SLUT *sLut,
	FILE *f,
	KOLIBA_ftype *ft
);

// Read a matrix from an open compatible file. It needs to be open
// for reading binary data. It remains open upon return, so
// the caller needs to close it. Returns mat on success, NULL
// on failure. If, however, mat is not NULL, its contents
// will be filled with the identity mat on failure.

KLBDC KOLIBA_MATRIX * KOLIBA_ReadMatrixFromCompatibleOpenFile(KOLIBA_MATRIX *mat, FILE *f, KOLIBA_ftype *ft);

#endif	// USECLIB



#ifdef __cplusplus
}
#endif

#endif	// defined(_SLTCOV_H_)
