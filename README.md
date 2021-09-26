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

The `-t` and `-T` switches tell `sltconv` to produce a `text` version
of the `Simple Look-Up Table` (which normally sports the `.sltt` file
extension). The only difference is in the level of information appended 
after the end of the table. So, if we use the `-t` switch with our
example (with the `-e 0.75` switch), we get,

```
sLut
3FBAFCED916872AE 3FBAFCED916872B6 3FBAFCED916872B8
BFA7058EE66D7669 3FAACA2F50D954D9 3FFD275CE899405C
3FC45E49730EFF7D 3FF0ACE90391D0C8 BFEC4A85AF0C93A2
3F81E6EF0BF688C0 3FEFA6D74A042887 3FEAA4966FF8DEBF
3FEC58C691A31789 BFB833A7E189B704 3FAFBCBDDDA12E70
3FE788CFF10F31CE BFC2E5BEE542BFA7 3FFC7573FE6FC2A4
3FEE10BB3C39C912 3FEAF3BF58C55C59 BFEDAE57835F8F12
3FE940C49BA5E356 3FE940C49BA5E34F 3FE940C49BA5E34F
# Converted from the matrix:
#
#	1.292780003 -0.0716073776 0.2005148743 -0.1405625
#	0.2666134396 1.084276624 0.07079743624 -0.1405625
#	0.05791335385 1.319358431 0.04441571479 -0.1405625
#
# With -75% efficacy.
#
```

But if we use the `-T` switch, the result is,

```
sLut
3FBAFCED916872AE 3FBAFCED916872B6 3FBAFCED916872B8
BFA7058EE66D7669 3FAACA2F50D954D9 3FFD275CE899405C
3FC45E49730EFF7D 3FF0ACE90391D0C8 BFEC4A85AF0C93A2
3F81E6EF0BF688C0 3FEFA6D74A042887 3FEAA4966FF8DEBF
3FEC58C691A31789 BFB833A7E189B704 3FAFBCBDDDA12E70
3FE788CFF10F31CE BFC2E5BEE542BFA7 3FFC7573FE6FC2A4
3FEE10BB3C39C912 3FEAF3BF58C55C59 BFEDAE57835F8F12
3FE940C49BA5E356 3FE940C49BA5E34F 3FE940C49BA5E34F

## Converted from "MQ.m34t" by sltconv, v.0.5.8

```

So, the `-t` switch is more informative. But if you want to
let everyone use a LUT you made, but want or need to keep
the method you created it with to yourself, then take
advantage of `-T` instead.

Last but not least, the `-c` switch forces `sltconv`
to output the `.cube` format. That is the default anyway,
but the switch is added so you can override any of the
other switches already given in a situation where you
cannot (or it is too much trouble) just delete them from
the command line.

## dblhex

The `dblhex` utility is strictly speaking not `libkoliba`
related (and does not bind to it, so it can be run even
without `libkoliba` installed on your system). But I wrote
it specifically to help with the task of editing the newly
textual file formats added to `libkoliba`.

As described above, these files store the floating-point
double values in hexadecimal notation which captures their
bit-by-bit representation inside a computer. For example,
the decimal number `1.0` is `3FF0000000000000` in hexadecimal,
while the decimal `0.0` is `0000000000000000`. The `Identity
Simple Look-Up Table` consists of just zeros and ones, and
would look like this in plain decimal:

```
0 0 0
0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
1 1 1
```

If you are familiar with binary notation, this looks very
much like the binary representation of the numbers `0` - `7`:

```
000 (= 0)
001 (= 1)
010 (= 2)
011 (= 3)
100 (= 4)
101 (= 5)
110 (= 6)
111 (= 7)
```

And if you think of the first digit in each row as representing
the `red` channel of a `LUT vertex`, the second number as representing
its `green` channel, and the third (rightmost) number the `blue`
channel, you will be able to surmise without memorizing, that in the
`000` line all three channels are `0`, so it represents the `black`
vertex, in the `001` line only the blue channel is `1`, so the line
represents the `blue` vertex, and so on, `010` = `green`, `011` =
`green+blue` = `cyan`, etc. In other words, the `Identity sLut` with
annotation is,

```
0 0 0 (Black)
0 0 1 (Blue)
0 1 0 (Green)
0 1 1 (Cyan)
1 0 0 (Red)
1 0 1 (Magenta)
1 1 0 (Yellow)
1 1 1 (White)
```

If this is clear, you will never have to look up this list again
because you will always be able to figure out which line represents
which vertex of the `Koliba` `Simple Look-Up Table`.

Then again, that is the `decimal` notation. In the `hexadecimal`
notation, the `identity.sltt` text file looks like,

```
sLut
0000000000000000 0000000000000000 0000000000000000
0000000000000000 0000000000000000 3FF0000000000000
0000000000000000 3FF0000000000000 0000000000000000
0000000000000000 3FF0000000000000 3FF0000000000000
3FF0000000000000 0000000000000000 0000000000000000
3FF0000000000000 0000000000000000 3FF0000000000000
3FF0000000000000 3FF0000000000000 0000000000000000
3FF0000000000000 3FF0000000000000 3FF0000000000000
```

That is the formatting produced by `libkoliba` functions to make
it easier for us biologicals to see the pattern of eight lines,
with three numbers each. But that formatting is not required. So,
if we just wrote those 8*3=24 numbers each on a separate line,
or all of them in one line, or whatever, `libkoliba` will understand
it. The only requirement is that the text start with the case-sensitive
signature of `sLut`, and be followed by those 24 numbers in their right
order expressed in the hexadecimal notation, all separated by some blank
space, such as space, tab, new line...

So this version is exactly the same as the above `identity.slt`,

```
sLut
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
3FF0000000000000
0000000000000000
3FF0000000000000
0000000000000000
0000000000000000
3FF0000000000000
3FF0000000000000
3FF0000000000000
0000000000000000
0000000000000000
3FF0000000000000
0000000000000000
3FF0000000000000
3FF0000000000000
3FF0000000000000
0000000000000000
3FF0000000000000
3FF0000000000000
3FF0000000000000
```

Or even this,

```
sLut 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000
3FF0000000000000 0000000000000000 3FF0000000000000 0000000000000000
0000000000000000 3FF0000000000000 3FF0000000000000
3FF0000000000000 0000000000000000
0000000000000000
	3FF0000000000000
		0000000000000000
			3FF0000000000000
				3FF0000000000000
					3FF0000000000000
				0000000000000000
			3FF0000000000000
		3FF0000000000000
	3FF0000000000000
```

The machine does not care (try to copy them from here and paste
them to a text file, separate file each time, then use `sltconv`
to show you the `.cube` version, which, by the way, uses a
different line order, but they will all give you the same `sltconv`
output). Of course the one shown first is much easier on us, humans.

To convert any group of `decimal` numbers to the `hexadecimal`
representation of them as floating-point `doubles`, just type
`dblhex` followed by those numbers on the command line. If you
want to type them as groups of three (or any other group), try
typing a comma (`,`) immediately after each third number. And by
_immediately_ I mean with no blank space after the number.

For example to produce a valid `identity.sltt` file, whether on
Unix or Windows or some other system, type,

```
$ echo sLut > identity.sltt
$ dblhex 0 0 0, 0 0 1, 0 1 0, 0 1 1, 1 0 0, 1 0 1, 1 1 0, 1 1 1 >> identity.sltt
```

The first line creates a new `identity.sltt` file and writes
`sLut` followed by a new line to it. The second line appends
all those hexadecimals, each on a separate line.

If you then try to convert it to the `.cube` format with `sltconv`,
you may be surprised by the result:

```
sltconv identity.sltt
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "identity.sltt"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_1D_SIZE 2
0 0 0
1 1 1

## Converted from "identity.sltt" by sltconv, v.0.5.8
```

Yes, `sltconv` has recognized it as the `Identity LUT` and
produced the smallest and simplest way of expressing it.

The `Identity LUT` (`ILUT`) is a very important starting point in creating
color effects, but their whole usefulness is in what we change
the `ILUT` to and how we go about it.

As an obvious example, suppose you were asked to edit some scenes
in a movie to give the actors a happy and healthy look, and to do
it in a subtle and non-obvious way. After all, just as we do not
ask the actors to overact but be natural, we do not want our
colorists to overcolor but be natural.

Regardless of the race, the color of the human skin is produced by just
different amounts of two types of melanin, and the result is somewhere
around the orange color, albeit of varying darkness.

So we want to create a `LUT` that will augment the `orange vertex`.
But wait... What orange vertex??? We have just eight `vertices`, black,
white, red, green, blue, cyan, magenta, and yellow!

Of course we do. But the whole point of having the vertices is to be able to
mix any color from them. Orange is just an equal amount of red and
yellow. All we need to do is enhance, very subtly, `red` and `yellow`.
But perhaps not in an equal strength because we want to avoid going too yellow.

In our exercise, then, we shall boost `yellow` a bit, `red` twice
as much as `yellow` and leave everything else at default. How do we
boom `red` (or any color)? Remember, in the LUT, the vertices other
than `black` and `white` consist of a `1` and two `0`s, or a `0` and two
`1`s. To strenghten the color controlled by a vertex, we have to _increase_
the `1`s and _decrease_ the `0`s. Just a little.

So let us change `red` from `1 0 0` to `1.2 -0.1 -0.1` and `yellow` from `1 1 0`
to `1.1 1.1 -0.05`. Everything else stays the same as in the `ILUT`. If, then,
we decide to call the new file `rus.sltt`, we create it by typing these two commands,

```
echo sLut > rus.sltt
dblhex 0 0 0, 0 0 1, 0 1 0, 0 1 1, 1.2 -0.1 -0.1, 1 0 1, 1.1 1.1 -0.05, 1 1 1 >> rus.sltt
```

We can view it in the `.cube` format,

```
$ sltconv rus.sltt
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "rus.sltt"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
0 0 0
1.2 -0.1 -0.1
0 1 0
1.1 1.1 -0.05
0 0 1
1 0 1
0 1 1
1 1 1

## Converted from "rus.sltt" by sltconv, v.0.5.8
```

And if you add the output file name, say `rus.cube`, you now have
a nice `LUT` that makes people look happy and healthy. Try it, it works!

Now suppose the people who asked you to do it were so impressed by your
skill they want you to produce the opposite effect in other scenes, where
the same actors are struggling, and do not look so happy and healthy.

OK, what do you do? Do you demote the `red` and `yellow` vertices now?
Or do you reset them to their defaults and elevate all the other color
vertices? Well, you can try, but neither would be the best solution.

I mean yes, you would probably restore `red` and `yellow` to their `ILUT`
values. But the next step should be doing the same as you did before, just not
to all the other vertices but only to the `complementary` vertices of `red` and
`yellow`. The complement of `red` is `cyan`. So we are going to change it
exactly how we changed `red` before and replace the default `cyan` values
from `0 1 1` to `-0.1 1.2 1.2`.

The opposite of yellow is `blue`. We will change it from `0 0 1` to
`-0.05 -0.05 1.1`. Perhaps we should save it all in the file called `sur.sltt`
since _sur_ is _rus_ backwards,

```
echo sLut > sur.sltt
dblhex 0 0 0, -0.05 -0.05 1.1, 0 1 0, -0.1 1.2 1.2, 1 0 0, 1 0 1, 1 1 0, 1 1 1 >> sur.sltt
```

Then we just make a `.cube` file from it (unless your software supports
`libkoliba` file formats directly),

```
$ sltconv sur.sltt
sltconv, v.0.5.8
Copyright 2019-2021 G. Adam Stanislav
All rights reserved

TITLE "sur.sltt"
DOMAIN_MIN 0 0 0
DOMAIN_MAX 1 1 1
LUT_3D_SIZE 2
0 0 0
1 0 0
0 1 0
1 1 0
-0.05 -0.05 1.1
1 0 1
-0.1 1.2 1.2
1 1 1

## Converted from "sur.sltt" by sltconv, v.0.5.8
```

So why not just type in the `.cube` files directly, with no need to go
hexadecimal? Because remember, `sltconv` has the `-e` flag. So if you find the
effects too strong, just re-run `sltconv` with an efficacy less than `1`.
Or if you find the effects too weak, opt for `sltconv` with an efficacy of more
than `1` (perhaps even a lot more). There are no limits!

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
