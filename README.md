
# Amiga Spec - Unit testing Amiga-like OS's #

The goal of this project is to gradually build up a selection of unit tests for Amiga-like OS's:

 * AmigaOS "Classic" (3.x in particular)
 * AmigaOS 4.x
 * AROS
 * MorphOS

The tests should, except where otherwise noted, aim to succeed or fail everywhere, and
provide a sort of executable "spec" for a baseline Amiga-like OS.

I will add tests here as part of my work on AROS primarily, as well as for other projects
where portability to other Amiga flavors is important to me, but submissions are welcome.

In general classic AmigaOS is taken as the benchmark for expected results, unless otherwise
noted (e.g. functionality that is only available on newer flavors).

Where possible, automatic determination of success or not should be the goal. That might
require some extra effort, e.g. for code that requires specific graphical output, it might
be neccessary to read that output back into a bitmap and compare it with a bitmap from
an image file.


