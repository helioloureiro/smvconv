smvconv
=======

A tools abandoned long ago designed to convert to smv movie format.
SMV stands for Sigmatel Motion Video and was largely used by devices
by Phillips.

This code was recovered from http://codu.org/smvconv/
which just disappeared.

The following description is from the original website.

smvconv
=======

This is a tool for creating .smv video files for portable media players on GNU/Linux. As it turns out, .smv is an extremely simple format. This encoder is based on mplayer and ffmpeg, and both are prerequisites for it to work.

An incredibly tiny format description is included in the tarball.

smvconv is distributed under the MIT/X11 license.

BIG FAT NOTE: This converter is EXTREMELY preliminary. In fact, I haven't even tested it - I don't have my player yet. It's based entirely on producing similar output to the (ridiculously terrible) Windows converters I could find.

[src/ smvconv 2007-11-06]

To build smvconv, just cd into the smvconv directory and type `make`. smvconv itself is a shell script, but it depends on smvcombine, which is a C program.

Limitations
===========

SMV is a constant-bit-rate format, but the underlying format (jpeg) is not. I suspect that the proprietary encoders make up for this by encoding at various qualities until a jpeg of the proper size is made. smvconv does not do this, it creates constant-quality jpegs. This causes wasted space in the output file.
