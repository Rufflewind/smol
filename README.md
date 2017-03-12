# smol: a tiny, uninteresting language

A weird pseudo-assembly language written over the course of a Saturday evening
while I was sick, tired, and bored.

“smol” is a bastardization of “Some Language”, for the lack of a better name.

The compiler is [written in its own language](smolc.smol), but the
[original compiler](smolc_orig.c) was written in C and is useful for
kickstarting the bootstrap process.  The original compiler has fewer features
and does not check its inputs as thoroughly (i.e. doesn't bother to at all) as
the self-hosted version.

There’s nothing particularly useful or impressive about this language.  It was
done entirely for the learning experience :)

The language is entirely documented within [smolc.smol](smolc.smol).

## Building

    make

## Usage

    ./smolc <input_file >output_file
