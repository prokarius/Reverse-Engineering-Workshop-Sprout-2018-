# Flags, `cmp` and logical jumps.

In a computer, on top of the 8 registers we introduced above, there also exists a
special register called the *flag register*. This register is in charge of taking
note when certain conditions in the code are being fulfilled. There are multiple
flags in that register, each may trigger during each instruction. Mainly there are
three flags that we concern ourselves with for the `jump` series of instructions.


## 3 main flags

First up, we talk about the *overflow flag* (`OF`). This flag triggers when the
value of the piece of data we are working with exceeds the number of bits available
in the register. That is, we cannot express the resulting number in the range
provided by the space of the variable.

Note that the overflow flag may only be set when adding two numbers with the same
sign, or when subtracting two numbers of different sign. In all other cases, it can
be shown that the resulting value would fit in the the range of the memory
available.

For example, when we subtract a large negative value from a large
positive value, the resulting value might be bigger than the largest positive
number that could be represented by the number of bits of the register, and hence
would result in an overflow, setting the flag.

Another flag of particular interest is the *zero* flag (`ZF`). It is set when the
results of an arithmetic operation results in a zero value. The next instruction is
usually followed by a `jz` instruction. This is because the `jz` instruction relies
on the zero flag to decide whether it should jump to the next value.

Another flag we should take note of are the *sign* flag (`SF`). This lets us know
whether the result of the previous instruction was positive or negative. If the
result is a negative number, then the sign flag would be set to 1. With the
three flags above, we can reason out a couple of how some of the instructions from
the `jump` series would rely on the flags to work.


## `cmp` instructions

A `cmp` instruction essentially takes the second parameter, and subtracts it from
the first instruction. The flag are then set in the same manner, however the results
are not stored. We can therefore try to reason out the logic in a jump instruction.

Take the `jle` instruction for example. Suppose we want to jump to a memory
location if the value in the `eax` register is less than or equal to the value in
the `edx` register. First we will need to do `cmp  eax, edx`. This subtracts the
value of `edx` from `eax`. If `eax` is less than or equal to `edx`, then the
result of doing `eax - edx` would be negative, or 0.

Which flags would be set if `eax - edx` is negative or 0? Well clearly, if the
zero flag is set, then we can be sure we will jump. How do we tell if `eax - edx`
is negative though? Consider the situation when both `edx` and `eax` are positive.
Then the result of `eax - edx` will be negative and will not trigger the overflow
flag. Then we have `SF = 1` and `OF = 0`. You can reason out in the same way when
both `eax` and `edx` are negative.

At the same time, if `edx` is a large positive number, and `eax` is a large
negative number, then it may be the case that `eax - edx` is so small that the
result underflows. Then `OF` will be triggered. Since the result of the `cmp`
instruction is positive, therefore `SF` would not be triggered.

Lastly, if `edx` is a large negative number, and `eax` is a large positive number.
It may be the case that `eax - edx` is so far positive that the result rolls over
and sets `OF`. However, in that case, the resulting value of the `cmp` instruction
is negative, it also sets `SF`.

From here we can see that we want to take the jump if `SF != OF` or if `ZF == 1`.

You can try to do the same reasoning for the other instructions in the `jump`
series. You can check your understanding at [this website][cmps]

[cmps]: http://unixwiz.net/techtips/x86-jumps.html


## `cmp` vs `test`

Now it might be the case where instead of seeing a `cmp` instruction before a `jump`
instruction, you see a `test` instruction. Truth of the matter is, the `test`
instruction isn't all that different from the `cmp` instruction. In some beginner
level textbooks, they even remark that `cmp` and `test` instructions are
interchangable when a `jz` instruction is concerned. However there are some small
subtle differences in the way they work.

A `cmp` instruction takes the second parameter and *SUBTRACTS* it from the first,
whereas in a `test` instruction, the *bitwise AND* operation is applied to both
parameters. In both cases, the result is discarded and not used again, but the side
effects of doing both these operations are reflected in the flag register. The
instruction sets some flags which will be read by the computer to determine if the
following `jump` instruction is to be taken.

Why would a program want to use the `test` instruction though? Remember that bitwise
operations are much faster than subtraction operations. When you will be using a
certain instruction many times over, even small differences might cause the program
to slow down quite a bit. Hence, the compiler sometimes would use `test` instead of
`cmp` instructions before a jump, just to speed up the program.


## Carry flag

Another flag that might show up is the *carry* flag (`CF`). This flag is set when
the result of an add operation causes there to be a overflow in the most significant
bit of the data. This means that the result of the operation would result in data
that exceeds the number of bits available in the register. That is, we cannot
express the resulting number in the range provided by the space of the variable.

Wait that sounds similar to the explaination I gave for the overflow flag. And you
would be correct. The overflow flag is similar to the carry flag, except that it
works with signed integer values. Suppose I am working with a 32-bit `int` variable.
That is, the numbers that I can represent need to be able to fit within 32-bits.

The carry flag will be triggered when the result of adding two values together
escapes into the 33rd bit. However, the overflow flag will be triggered when the
values cause the 32nd bit to flip.

Why would having such a distinction be useful? It is because this allows us to
make a distinction between when we are using signed binary digits or when we are
using unsigned binary digits.


## Signed vs Unsigned 

So in the main lesson, we talked about how numbers are being stored in memory, and
I gave an example of when the values overflowed or underflowed. I ended the section
saying that computers have no way to *"know whether a number is supposed to be a
negative number, or a really big positive number"*. Well that isn't really the case.
This is where signed and unsigned integers come in.

Now, remember how when dealing with negative numbers, we mentioned that we can just
put a negative sign in front of a number to turn it negative. However, a computer
doesn't have negative signs. It only works with `1`s and `0`s. Where will it get its
negative signs from? Obviously it'll have to do it with the `1`s and `0`s available
to it!

Can we sacrifise one bit in order to allow the values that we can hold include the
sign bit? Perhaps we can consider using the most significant bit as the sign bit.
That is, if the bit is `0`, then we can say the value is positive, but if the bit is
set (`1`), then we say the value is negative. That means that we scrifice one bit in
order to expand our number line downwards, into the negative region. We can now only
express smaller positive numbers, but in return we get to use negative numbers.

This is the the meaning of a signed value. An unsigned value means that we do not
really care about having a negative value. We only concern ourselves with positive
values. This allows us a larger range of positive values, at the expense of not
allowing negative values to exist.


## Negative numbers

Which leads to the next point: How are negative numbers expressed? In chapter 0, I
gave a brief overview of how numbers are expressed in binary. That is the case for
positive integers. Above, we mentioned how we need to sacrifice the most significant
bit to represent the sign of the number, let's do that for a simple 4 bit number:

| bit |  3  |  2  |  1  |  0  |
|-----|-----|-----|-----|-----|
|value| sign|  4  |  2  |  1  |
|state|  0  |  1  |  1  |  0  |

If we want to represent the number `6`, we will need to turn on bits 0 and 2, so that
the value we get is `1*4 + 1*2 + 0*1`. Then we can say that to represent the number
5 in binary, we say `0b0110`.


### Sign magnitude approach

Now what happens if we want to represent a negative number? You might first think
that maybe we can do the same as what we do with decimal numbers: You form the
value with the first 3 bits, then add on the negative sign, as such for `-6`:

| bit |  3  |  2  |  1  |  0  |
|-----|-----|-----|-----|-----|
|value| sign|  4  |  2  |  1  |
|state|  1  |  1  |  1  |  0  |

That way, `-5` would be represented as `0b1110`. Yes, this is a valid way. However,
lets think about the value `0b0000` and `0b1000`. These two values correspond to
`+0` and `-0` respectively. This kinda makes the system a little distasteful, we are
wasting one number's worth of representation. It is also the case that by having two
different binary representation of the same value could cause problems, as the two
binary numbers might behave differently in different cases. This is called the
*signed zero* problem. 

This and the fact that arithmetic is harder to perform with such a system is the
reason why we consider another representation.


### Ones Complement

Now, perhaps we can flip all the bits (changing all bits that are `1`s into `0`s and
vice verse) of a number to signify that it is a negative number. By flipping all the
bits, it has a bonus effect of flipping the sign bit as well. This is what we call
the *ones' complement* (1s complement) system. For example, this is how you would
represent the value of `-6` in the ones' complement system:

First we find the value of `+6` in binary, which from above you know is `0b0110`.
Now, you just have to flip all the bits around to get `0b1001`.

|  bit |  3  |  2  |  1  |  0  |
|------|-----|-----|-----|-----|
| state|  0  |  1  |  1  |  0  |
|-state|  1  |  0  |  0  |  1  |

This seems like a good system, it is simple for a computer to calculate a negative
number. However, there is still a problem: Namely there is still the signed zero
problem here as well. Note that `0b0000` and `0b1111` are `+0` and `-0`
respectively. Is there a better way?


### Twos' Complement

Fortunately, yes, there is a better way. Take the numbers that we get from the ones'
complement and add 1 to them. This would give you the representation of it in twos'
complement. So from above, `-6` would then be `0b1010`


Why is this better? Remember when we were counting down from `5` in the previous
lesson, and when we hit `0`, we decided to keep borrowing from the more significant
digits in order to count backwards? Oh you don't remember eh... OK nevermind, here
you go:

```
Binary : 101... 100... 11... 10... 1... 0... 1111... 1110... 1101...
Decimal:  5 ...  4 ...  3...  2... 1... 0...  -1 ...  -2 ...  -3 ...
```

In the same way this system can be seen as such. This is essentially the twos'
complement system. Note that there isn't the presence of a `-0` in this system as
the only representation of `0` is that of `0b0000`. This solves the signed zero
problem.

Similarly, consider how to do arithmetic. A number added to its negative should
result in a value of `0`. Does it work here? Let's try with `6 + -6`:

We have `0b0110 + 0b1010 = 0b10000` But because we can only store the bottom 4 bits,
we end up with `0b0000`. This is equal to `0`! And you can try for any other number,
the twos complement system will always gaurantee that any number added with its
negative would result in `0`. This is because in the ones' complement system, the
sum of a number and its negative would result in `0b1111`. Since in the twos'
complement system, we add one to the negative value, we therefore get back `0b0000`.

So what is the range of the values that can be expressed by 4 bits in the twos'
complement system? Notice that for positive values, it goes up to `0b0111`, which is
`+7`. How about negatives? We know that `-7 = ~0b0111 + 1 = 0b1001`, which means we
can go one lower: `0b1000 = -8`! Therefore the range of values goes from `-8 to 7`

Generalising, we can conclude that in the twos' complement system, the values range
from `-2^n to 2^n - 1` for an n+1 bit binary number.
