# Basic Reverse Engineering Course (for Sprout 2018)

A basic 2h workshop on Reverse Engineering, created for Sprout for NUSHS 2018.
As this is a basic introduction, no advanced concepts would be covered. This
workshop would not cover pwn topics, just basic reverse engineering. More advanced
topics could be found under @nn_amon's [linux-exploitation-course][amoncourse].

[amoncourse]: https://github.com/nnamon/linux-exploitation-course


## Pre-Requisites

A computing background would help with the initial understanding, however it is
not mandatory as the basics of programming in C has been covered in chapter 0.

A working copy of [IDA-PRO][idapro] could be downloaded for free. This would aid in
the reverse engineering process. Pen and paper x86 code would be given out on the
day itself for student who wants to decompile code by hand.

[idapro]: https://www.hex-rays.com/products/ida/support/download_freeware.shtml


## Online Tools

Alternatively, several free online decompilers are also available. I would highly
recommend [onlinedisassembler][oda] as a simple starting point. Their graph view
allows code to the traced very easily.

At the same time, an online C compiler would be useful as well, and for that
I highly recommend [repl.it][replt]. It allows for very speedy compilation and
prototyping.

Lastly, for the best of both worlds which shows you the disassembly at the same time
as you're typing it, I highly recommend [godbolt][godbolt]. They even colour code
the pieces of code to show you which bits of code gets turned into which bits of
machine language.

[oda]: https://onlinedisassembler.com/odaweb/
[replt]: https://repl.it/
[godbolt]: https://godbolt.org/


## Syllabus

0. [An Intoduction to C][lesson0]
1. [Reading x86][lesson1]
2. [Functions][lesson2]
3. [Loops and recursion in x86][lesson3]
4. Final Exercise -- To be released

[lesson0]: ./lessons/0_introduction/lessonplan.md
[lesson1]: ./lessons/1_reading_x86/lessonplan.md
[lesson2]: ./lessons/2_functions/lessonplan.md
[lesson3]: ./lessons/3_loops/lessonplan.md

