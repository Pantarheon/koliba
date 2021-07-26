# KOLIBA

Sample applications to illustrate the use of the `libkoliba` library of
parsimonious color correction/grading routines.

Until I get a chance to write a thorough explanation, the
[koliba.h](./src/koliba.h) file is filled with explanatory
comments.

To install `koliba`, please read [INSTALL.md](./INSTALL.md) first.

## sltconv

This utility converts from several of the binary file formats
created by the `koliba` library into a simple `LUT` in the 
`.cube` format.

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
