#	Copyright 2019-2021 G. Adam Stanislav
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
OBJS=sltconv.o pullmat.o invertmat.o effimat.o farbosity.o \
	ConvertRecs.o RotationMatrix.o quintluts.o \
	eryslut.o dblhex.o MatrixChain.o dichromatic.o \
	matconv.o matspan.o 
APPS=sltconv pullmat invertmat effimat \
	ConvertRecs RotationMatrix quintluts \
	eryslut dblhex MatrixChain dichromatic \
	matconv matspan 

build-all: $(APPS)

rebuild: clean build

sltconv: sltconv.o
	$(CC) $(CFLG) -o sltconv sltconv.o -L$(PREFIX)/lib -lkoliba

matconv: matconv.o
	$(CC) $(CFLG) -o matconv matconv.o -L$(PREFIX)/lib -lkoliba

matspan: matspan.o
	$(CC) $(CFLG) -o matspan matspan.o -L$(PREFIX)/lib -lkoliba

pullmat: pullmat.o
	$(CC) $(CFLG) -o pullmat pullmat.o -L$(PREFIX)/lib -lkoliba

invertmat: invertmat.o
	$(CC) $(CFLG) -o invertmat invertmat.o -L$(PREFIX)/lib -lkoliba

effimat: effimat.o
	$(CC) $(CFLG) -o effimat effimat.o -L$(PREFIX)/lib -lkoliba

farbosity: farbosity.o
	$(CC) $(CFLG) -o farbosity farbosity.o  -L$(PREFIX)/lib -lkoliba

ConvertRecs: ConvertRecs.o
	$(CC) $(CFLG) -o ConvertRecs ConvertRecs.o -L$(PREFIX)/lib -lkoliba

RotationMatrix: RotationMatrix.o
	$(CC) $(CFLG) -o RotationMatrix RotationMatrix.o -L$(PREFIX)/lib -lkoliba -lm

quintluts: quintluts.o
	$(CC) $(CFLG) -o quintluts quintluts.o -L$(PREFIX)/lib -lkoliba -lm

eryslut: eryslut.o
	$(CC) $(CFLG) -o eryslut eryslut.o -L$(PREFIX)/lib -lkoliba

dblhex: dblhex.o
	$(CC) $(CFLG) -o dblhex dblhex.o

MatrixChain: MatrixChain.o
	$(CC) $(CFLG) -o MatrixChain MatrixChain.o -L$(PREFIX)/lib -lkoliba

dichromatic: dichromatic.o
	$(CC) $(CFLG) -o dichromatic dichromatic.o -L$(PREFIX)/lib -lkoliba

sltconv.o: sltconv.c koliba.h
	$(CC) $(CFLG) -c sltconv.c -I$(PREFIX)/include

matconv.o: matconv.c koliba.h
	$(CC) $(CFLG) -c matconv.c -I$(PREFIX)/include

matspan.o: matspan.c koliba.h
	$(CC) $(CFLG) -c matspan.c -I$(PREFIX)/include

pullmat.o: pullmat.c koliba.h
	$(CC) $(CFLG) -c pullmat.c -I$(PREFIX)/include

invertmat.o: invertmat.c koliba.h
	$(CC) $(CFLG) -c invertmat.c -I$(PREFIX)/include

effimat.o: effimat.c koliba.h
	$(CC) $(CFLG) -c effimat.c -I$(PREFIX)/include

farbosity.o: farbosity.c koliba.h
	$(CC) $(CFLG) -c farbosity.c -I$(PREFIX)/include

ConvertRecs.o: ConvertRecs.c koliba.h
	$(CC) $(CFLG) -c ConvertRecs.c -I$(PREFIX)/include

RotationMatrix.o: RotationMatrix.c koliba.h
	$(CC) $(CFLG) -c RotationMatrix.c -I$(PREFIX)/include

quintluts.o: quintluts.c koliba.h
	$(CC) $(CFLG) -c quintluts.c -I$(PREFIX)/include

eryslut.o: eryslut.c koliba.h
	$(CC) $(CFLG) -c eryslut.c -I$(PREFIX)/include

MatrixChain.o: MatrixChain.c koliba.h
	$(CC) $(CFLG) -c MatrixChain.c -I$(PREFIX)/include

dichromatic.o: dichromatic.c koliba.h
	$(CC) $(CFLG) -c dichromatic.c -I$(PREFIX)/include

dblhex.o: dblhex.c
	$(CC) $(CFLG) -c dblhex.c

install-sltconv: sltconv
	install -p -s sltconv $(PREFIX)/bin

install-matconv: matconv
	install -p -s matconv $(PREFIX)/bin

install-matspan: matspan
	install -p -s matspan $(PREFIX)/bin

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

install-quintluts: quintluts
	install -p -s quintluts $(PREFIX)/bin

install-eryslut: eryslut
	install -p -s eryslut $(PREFIX)/bin

install-dblhex: dblhex
	install -p -s dblhex $(PREFIX)/bin

install-MatrixChain: MatrixChain
	install -p -s MatrixChain $(PREFIX)/bin

install-dichromatic: dichromatic
	install -p -s dichromatic $(PREFIX)/bin

install-all: install-sltconv install-matconv install-matspan install-pullmat install-invertmat install-effimat install-ConvertRecs install-RotationMatrix install-quintluts install-eryslut install-dblhex install-MatrixChain install-dichromatic

clean:
	rm -f $(OBJS) $(APPS)


