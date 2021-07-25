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

build-all: install sltconv pullmat invertmat effimat ConvertRecs

rebuild: clean build-all

sltconv: sltconv.o
	$(CC) $(CFLG) -o sltconv sltconv.o -L$(PREFIX)/lib -lkoliba

pullmat: pullmat.o
	$(CC) $(CFLG) -o pullmat pullmat.o -L$(PREFIX)/lib -lkoliba

invertmat: invertmat.o
	$(CC) $(CFLG) -o invertmat invertmat.o -L$(PREFIX)/lib -lkoliba

effimat: effimat.o
	$(CC) $(CFLG) -o effimat effimat.o -L$(PREFIX)/lib -lkoliba

ConvertRecs: ConvertRecs.o
	$(CC) $(CFLG) -o ConvertRecs ConvertRecs.o -L$(PREFIX)/lib -lkoliba

RotationMatrix: RotationMatrix.o
	$(CC) $(CFLG) -o RotationMatrix RotationMatrix.o -L$(PREFIX)/lib -lkoliba -lm

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

install-all: install-sltconv install-pullmat install-invertmat install-effimat install-ConvertRecs install-RotationMatrix

clean:
	rm -f sltconv.o sltconv sltconv.o pullmat pullmat.o invertmat invertmat.o effimat effimat.o ConvertRecs ConvertRecs.o RotationMatrix RotationMatrix.o


