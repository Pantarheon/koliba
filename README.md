# KOLIBA

Sample applications to illustrate the use of the `libkoliba` library of
parsimonious color correction/grading routines.

Until I get a chance to write a thorough explanation, the
[koliba.h](./src/koliba.h) file is filled with explanatory
comments.

To install `koliba`, please read [INSTALL.md](./INSTALL.md) first.

The Koliba library (`libkoliba`) defines a number of file formats.
These were originally created for my `Palette Mallet` `OFX plug-in`
to allow exporting various look-up tables, matrices, etc from the
plugin and to import them back.

Since `OFX plug-ins` tend to process millions, even hundreds of
millions and more pixels, they need to work as fast as possible.
For that reason, all those file formats are binary.

In 2021, I started adding text versions of these formats, some of
them anyway but more will follow, to allow exchanging look-up tables
over text communication channels (including online fora, many of which
do not allow the uploads of binary data).

The data in these files consists of double-precision floating point
numbers. Because computers use binary notation but humans use decimal
numbers, exporting floating-point numbers typically involves converting
them from binary to decimal and losing some precision. Then importing
them back involves converting from decimal back to binary numbers, again
losing (yet more) precision.

For the `libkoliba` data such a loss of precision is unacceptable,
therefore, even the text files preserve the binary precision by
converting the data from binary to hexadecimal and back.
This preserves the precision, albeit makes it hard for humans to
read and write.

On the other hand, because `libkoliba` only reads as much from these
text files as it needs to obtain its data, it is possible to add
comments in any human language _after_ the data.

For example, the `MQ` matrix was produced by chaining several matrices
using the `MatrixChain` utility and saved in the text `MQ.m34t file`.
It looked like this:

```
m3x4
3FF4AF3A15B14F33 BFB254DC70F21066 3FC9AA78ADBF73EC BFC1FDF3B645A1C9
3FD11031D0FB633E 3FF1593272064884 3FB21FC7E14FB5B8 BFC1FDF3B645A1CF
3FADA6D1B194F400 3FF51C1796266BFB 3FA6BDA814E1229E BFC1FDF3B645A1D0

# The Chain:
# ==========
#
# ChromaFun.chrt
# rotate17.chrm
# RedComplement.m3x4
# Anthropochromatic.m3x4
# More Saturation (Rec 2020).m3x4
```

Thanks to those comments, I will always know how I created the MQ matrix
because all I have to do is to read the extra information (by the way `libkoliba`
ignores all of that extra information, so the `#` at the start of each
line is not needed, but it is customary to start comments with it).

## sltconv

This utility converts from several of the binary and text file formats
created by the `koliba` library into a simple `LUT` in the 
`.cube` format.

For example, we can convert the aforementioned `MQ.m34t`,

```
$ sltconv MQ.m34t
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "MQ.m34t"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
# Converted from the matrix:
#
#       1.292780003 -0.0716073776 0.2005148743 -0.1405625
#       0.2666134396 1.084276624 0.07079743624 -0.1405625
#       0.05791335385 1.319358431 0.04441571479 -0.1405625
#
-0.1405625 -0.1405625 -0.1405625
1.152217503 0.1260509396 -0.08264914615
-0.2121698776 0.9437141241 1.178795931
1.080610126 1.210327564 1.236709285
0.0599523743 -0.06976506376 -0.09614678521
1.352732378 0.1968483759 -0.03823343136
-0.0116550033 1.01451156 1.223211646
1.281125 1.281125 1.281125

## Converted from "MQ.m34t" by sltconv, v.0.5.8
```

Of course, normally we want to save it in a file, so we would
type this, instead,

```
$ sltconv MQ.m34t MQ.cube
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved
```

And if we then `cat` (or `type` on Windows) `MQ.cube`,

```
TITLE "MQ.m34t"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
# Converted from the matrix:
#
#       1.292780003 -0.0716073776 0.2005148743 -0.1405625
#       0.2666134396 1.084276624 0.07079743624 -0.1405625
#       0.05791335385 1.319358431 0.04441571479 -0.1405625
#
-0.1405625 -0.1405625 -0.1405625
1.152217503 0.1260509396 -0.08264914615
-0.2121698776 0.9437141241 1.178795931
1.080610126 1.210327564 1.236709285
0.0599523743 -0.06976506376 -0.09614678521
1.352732378 0.1968483759 -0.03823343136
-0.0116550033 1.01451156 1.223211646
1.281125 1.281125 1.281125

## Converted from "MQ.m34t" by sltconv, v.0.5.8
```

We are now ready to use it in any software that understands
the `.cube` file format, antiquated though it be, even if it knows
nothing about `libkoliba`.

You can even copy it from here, paste it to a new file in a
`plain text` editor, save it as `MQ.cube` and test it in whatever
image or video editor you use.

> If you are not familiar with matrices, the above matrix
> means that any pixel `[r,g,b]` should be converted to
> the pixel `[r',g',b']` by using the following three equations:
>
> ```
> r' = 1.292780003r   - 0.0716073776g + 0.2005148743b  -0.1405625
> g' = 0.2666134396r  + 1.084276624g  + 0.07079743624b -0.1405625
> b' = 0.05791335385r + 1.319358431g  + 0.04441571479b -0.1405625
> ```

Use the `-e` command line argument to change the `efficacy` of
the look-up table. To make the effect stronger, use an efficacy
greater than `1`. To make it weaker, use an efficacy greater than
`0` and lesser than `1`. To get the opposite effect, try the efficacy
of `-1`. You can use any number. Just bear in mind that venturing
outside the `0` to `1` range is generally not what the designer of the
look-up table had in mind (not that it should stop you).

For example,

```
$ sltconv MQ.m34t -e 1.2
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "MQ.m34t"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
# Converted from the matrix:
#
#       1.292780003 -0.0716073776 0.2005148743 -0.1405625
#       0.2666134396 1.084276624 0.07079743624 -0.1405625
#       0.05791335385 1.319358431 0.04441571479 -0.1405625
#
# With 120% efficacy.
#
-0.168675 -0.168675 -0.168675
1.182661004 0.1512611276 -0.09917897538
-0.2546038531 0.9324569489 1.414555118
1.096732151 1.252393077 1.484051142
0.07194284916 -0.08371807651 -0.3153761423
1.423278853 0.2362180511 -0.2458801176
-0.01398600396 1.017413872 1.267853975
1.33735 1.33735 1.33735

## Converted from "MQ.m34t" by sltconv, v.0.5.8
```

and,

```
sltconv MQ.m34t -e -0.75
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "MQ.m34t"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
# Converted from the matrix:
#
#       1.292780003 -0.0716073776 0.2005148743 -0.1405625
#       0.2666134396 1.084276624 0.07079743624 -0.1405625
#       0.05791335385 1.319358431 0.04441571479 -0.1405625
#
# With -75% efficacy.
#
0.105421875 0.105421875 0.105421875
0.8858368725 -0.09453820473 0.06198685961
0.1591274082 1.042214407 -0.8840969485
0.9395424057 0.8422543272 -0.9275319639
-0.04496428073 0.05232379782 1.822110089
0.7354507168 -0.1476362819 1.778675074
0.008741252472 0.9891163297 0.8325912654
0.78915625 0.78915625 0.78915625

## Converted from "MQ.m34t" by sltconv, v.0.5.8
```

If you run `sltconv` with no command line switches, it will say,

```
Usage: sltconv [-i] input [[-o] output] [-t|T|s|c] [-e efficacy]
```

The `[-t|T|s|c]` switches are mutually exclusive. If you use
any of them more than once, the one used last overrides the rest.

The `-s` switch instructs `sltconv` to output the `Simple Look-Up
Table` binary format, which is the main workhorse of the `Koliba
Library`. You should tell it the output file name, too, because
otherwise `sltconv` will just write it to `stdout`. This is on
purpose because in operating systems other than Windows it is
possible to pipe the binary output of one program as the input
of another.

At any rate, the file should have the extension `.sLut`
(pronounced in English the same as `salute`, of course). `sltconv`
does not care what extension you use, but if you stick with `.sLut`,
every `libkoliba` user will expect the file to contain the binary
version of the `Simple Look-Up Table`.

## ConvertRecs

A very simple example of using the `koliba` library to create
a set of matrices to convert among the various video standard
recs.

It takes no command line parameters, you just run it once and it
produces the following outputs in current directory:

```
ConvertRec2020ToRec601.m3x4
ConvertRec2020ToRec709.m3x4
ConvertRec601ToRec2020.m3x4
ConvertRec601ToRec709.m3x4
ConvertRec709ToRec2020.m3x4
ConvertRec709ToRec601.m3x4
```

The names of these matrices explain what they are for. For example,
if you have some old video footage shot with the prehistoric
[Rec. 601](https://en.wikipedia.org/wiki/Rec._601) standard
and you want to use it in a new video edited with the
[Rec. 2020](https://en.wikipedia.org/wiki/Rec._2020) standard,
you just apply the matrix in `ConvertRec601ToRec2020.m3x4` to the old
video and it will now conform to Rec. 2020. Of course, it will not increase
its resultion, it will just help it look on modern video monitors
the way it was originally meant to be.

You will probably want to convert it to the `.cube` `LUT` format
first, like this:

```
sltconv ConvertRec601ToRec2020.m3x4 ConvertRec601ToRec2020.cube
```

## effimat

It illustrates how to apply specific `efficacy` to a matrix, to make
its effect stronger or weaker.

```
effimat input.m3x4 output.m3x4 efficacy
```

If you do not specify the efficacy, it will use 0.5.

## invertmat

It inverts a matrix if it is invertible.

## pullmat

If a simple `LUT` represents a matrix, this will extract it and write
it to a `.m3x4` file.

## RotationMatrix

This will create a chromatic matrix that will rotate the colors by a specified angle,
potentially also adjust other values, such as magnitude, saturation, black
and white point.
