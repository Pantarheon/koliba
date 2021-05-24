#	Copyright 2019 G. Adam Stanislav
#	All rights reserved
#
#	Redistribution and use in source and binary forms,
#	with or without modification, are permitted provided
#	that the following conditions are met:
#
#	1. Redistributions of source code must retain the
#	above copyright notice, this list of conditions
#	and the following disclaimer.
#
#	2. Redistributions in binary form must reproduce the
#	above copyright notice, this list of conditions and
#	the following disclaimer in the documentation and/or
#	other materials provided with the distribution.
#
#	3. Neither the name of the copyright holder nor the
#	names of its contributors may be used to endorse or
#	promote products derived from this software without
#	specific prior written permission.
#
#	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
#	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
#	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CC=gcc
CFLG=-O3
CFLAGS=$(CFLG) -fPIC
ASFLAGS=--gstabs
PREFIX=/usr

objects = 8bitbuff.o abgr8xyz.o anachroma.o apppx.o appxyz.o argb8xyz.o \
	bgra32xyz.o bgra8xyz.o bindbxyz.o bindoxyz.o bindwxyz.o bindxyz.o \
	byte255.o calcsum.o cb2mat.o \
	cbreset.o cflthead.o checksum.o \
	chrmhead.o chrmreset.o chroma.o chrreset.o cmyx.o colmask.o \
	dblsrgb.o deffiset.o diachroma.o dichroma.o dicrhead.o effiflut.o \
	effilut.o effinc.o effinv.o effiset.o effislut.o erythropia.o \
	externpx.o externxyz.o \
	fflut.o findex.o fixdbl.o \
	fixslut.o flbinflut.o flinflut.o fltcube.o fltismat.o fltscube.o flutms.o \
	flutslut.o flutter.o flwinflut.o flyxyz.o frange.o fund.o gainslut.o \
	gamma.o gcomp.o gray2flut.o gray2slut.o gsmat.o \
	idchr.o idmallet.o idmat.o \
	idplt.o idplut.o idsfac.o idslut.o indbxyz.o indcxyz.o indoxyz.o indwxyz.o indxyz.o interflut.o \
	intermat.o interpol.o interslt.o intslut.o \
	invmat.o invslut.o isidflut.o kplthead.o ldxapply.o ldxhead.o ldxxyz.o \
	libver.o liftslut.o linsrgb.o m3x4head.o mal2slut.o malinit.o malsinit.o \
	mals2slut.o mat2cb.o mat3x3.o \
	matreset.o matrixgain.o matrixlift.o matto3x3.o \
	mkcube.o mkidcube.o mkpslut.o mmals2slut.o mofarslt.o \
	msflt.o msslt.o mulmats.o mulsluts.o \
	nan.o natcon.o \
	nmatrow.o nofarba.o nosvit.o offsetslut.o \
	ones.o partslt.o pi.o plt2slt.o pltreset.o \
	pltvalid.o polypx.o polyxyz.o \
	plut2slt.o purcol.o purery.o px32xyz.o px8xyz.o readchrm.o \
	readm3x4.o readslut.o rec601.o rec709.o rec2020.o \
	recrec.o rgb2flut.o rgb2slut.o rgba8srgb.o rgbslut.o rgbycc.o \
	sinterpol.o slt2plut.o slt2vert.o \
	sltcfs.o sltfactors.o sltismat.o sltms.o sltreset.o \
	slut2cube.o sluthead.o slutis1d.o srgba8bytes.o \
	tetramat.o tintflut.o unindxyz.o unindbxyz.o unindoxyz.o unindwxyz.o \
	valcbtitle.o vert2slt.o vertis1d.o vertismat.o vertpixl.o \
	vertsamp.o windbxyz.o windoxyz.o windwxyz.o windxyz.o writecflt.o writechrm.o writem3x4.o \
	writeslut.o xtrctch.o xyz32bgra.o xyz32px.o xyz8abgr.o xyz8argb.o xyz8bgra.o xyz8px.o \
	yccmat.o yccrgb.o  yshift.o yunshift.o zeroes.o

lib:	libkoliba.so

build-all: install sltconv pullmat invertmat effimat ConvertRecs

rebuild: clean build-all

sltconv: sltconv.o lib
	$(CC) $(CFLG) -o sltconv sltconv.o -L$(PREFIX)/lib -lkoliba

pullmat: pullmat.o lib
	$(CC) $(CFLG) -o pullmat pullmat.o -L$(PREFIX)/lib -lkoliba

invertmat: invertmat.o lib
	$(CC) $(CFLG) -o invertmat invertmat.o -L$(PREFIX)/lib -lkoliba

effimat: effimat.o lib
	$(CC) $(CFLG) -o effimat effimat.o -L$(PREFIX)/lib -lkoliba

ConvertRecs: ConvertRecs.o lib
	$(CC) $(CFLG) -o ConvertRecs ConvertRecs.o -L$(PREFIX)/lib -lkoliba

RotationMatrix: RotationMatrix.o lib
	$(CC) $(CFLG) -o RotationMatrix RotationMatrix.o -L$(PREFIX)/lib -lkoliba -lm

libkoliba.so: $(objects)
	$(CC) --shared -o libkoliba.so $(objects) -lm

install: lib
	install -p -s libkoliba.so $(PREFIX)/lib
	cp -p koliba.h $(PREFIX)/include
	chown root:root $(PREFIX)/include/koliba.h

sltconv.o: sltconv.c koliba.h
	$(CC) $(CFLG) -c sltconv.c -I$(PREFIX)/include

pullmat.o: pullmat.c koliba.h
	$(CC) $(CFLG) -c pullmat.c -I$(PREFIX)/include

invertmat.o: invertmat.c koliba.h
	$(CC) $(CFLG) -c invertmat.c -I$(PREFIX)/include

effimat.o: effimat.c koliba.h
	$(CC) $(CFLG) -c effimat.c -I$(PREFIX)/include

ConvertRecs.o: ConvertRecs.c koliba.h
	$(CC) $(CFLG) -c ConvertRecs.c -I$(PREFIX)/include

RotationMatrix.o: RotationMatrix.c koliba.h
	$(CC) $(CFLG) -c RotationMatrix.c -I$(PREFIX)/include

%.o: %.c koliba.h
	$(CC) -c $(CFLAGS) $< -o $@

install-sltconv: sltconv
	install -p -s sltconv $(PREFIX)/bin

install-pullmat: pullmat
	install -p -s pullmat $(PREFIX)/bin

install-invertmat: invertmat
	install -p -s invertmat $(PREFIX)/bin

install-effimat: effimat
	install -p -s effimat $(PREFIX)/bin

install-ConvertRecs: ConvertRecs
	install -p -s ConvertRecs $(PREFIX)/bin

install-RotationMatrix: RotationMatrix
	install -p -s RotationMatrix $(PREFIX)/bin

install-all: install install-sltconv install-pullmat install-invertmat install-effimat install-ConvertRecs install-RotationMatrix

clean:
	rm -f sltconv.o $(objects) sltconv sltconv.o pullmat pullmat.o invertmat invertmat.o effimat effimat.o ConvertRecs ConvertRecs.o RotationMatrix RotationMatrix.o libkoliba.so


