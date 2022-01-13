# uxnatr
Port of the uxn virtual machine to Atari computers (800/1200XL).

This project's objective is to implement an interpreter (and possibly a compiler of sorts)
to run uxntal code written for the Uxn virtual machine on Atari computers such as the 800/1200XL
(based on the 6502 processor).

## Interpreter

The interpreter works by parsing an uxntal program and converting its instructions to 6502
instructions. Whenever devices or specific memory addresses are accessed in uxntal, the
interpreter will perform the necessary conversions to make the Atari hardware do a similar
thing (e.g. change the background colour, make a noise, print something). Due to the hardware
limitations of Atari computers, not all features of the uxn virtual machine can be supported.

## Compiler(-ish)

The compiler will go one step ahead and, instead of parsing the code and running conversions
on the fly, will emit 6502 Assembly for each operation being executed in the source uxntal
code. One can think of it as an inlined interpreter. Certain optimizations can be performed
to reduce code size and etc., although that is not a primary goal of this experiment.
