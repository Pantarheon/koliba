# Installing Koliba

The way to install the koliba library depends on the
operating system.

## 64-bit Windows

You can simply download the precompiled binaries from
the [win64](./win64/) directory, place them in a folder,
and add the folder to the `PATH` environment variable.

Or you can compile it from the source code by changing
to the [src](./src/) folder and running
[win64make.bat](./src/win64make.bat) like this,

```
cd src
win64make
```

This assumes you have the Microsoft C compiler (`cl`).

Additionally, if you want to use the koliba library in your
own programs, you will also need to download [koliba.h](./src/koliba.h).

## Raspberry Pi

On the 32-bit `raspi` system you can take advantage
of parts of the code being re-written in the `arm-eabi-32`
assembly language. To replace the corresponding C code
with assembly language, change to the [src](./src)
directory and type,

``` /bin/sh
chmod +x arm-eabi-32-prep.sh
./arm-eabi-32-prep-sh
```

Then use `make` as described in the Unix/Linux section below.

## Unix/Linux and similar

Switch to the [src](./src) directory. Then if all you want
is to compile and install the koliba library, type,

``` /bin/sh
sudo make install
```

If you also want the included utility programs, type,

``` /bin/sh
sudo make install-all
```
