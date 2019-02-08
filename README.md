# Subtilis

[![Build Status](https://travis-ci.org/markdryan/subtilis.svg?branch=master)](https://travis-ci.org/markdryan/subtilis)


A BASIC compiler for retro computers.

BASIC is over 50 years old and may not be the most respected of computer
languages, but it, at least the BBC variant of BASIC, is not that bad really.
It's just missing a few things, like structures, maps, memory management, a
standard library, function pointers, vectorization, closures and a decent
compiler and tool chain.  There's a suspicion that adding these features to the
language in a natural way without compromising its simplicity, might result in
quite a nice programming environment.  At least this is the theory to be put to
the test in the Subtilis project.

The tentative plan for the project is to first create a compiler for a subset of
the existing BBC BASIC V features and then figure out a way of adding modern
constructs to the lanuage.  To start with there will probably only be two target
OSes, RISCOS 3 and RISCOS 4, and one CPU family, ARM 2 or greater.  Ultimately
the goal is to create a backend for the native ARM processor mode of PiTubeDirect,
but this is a long way off.