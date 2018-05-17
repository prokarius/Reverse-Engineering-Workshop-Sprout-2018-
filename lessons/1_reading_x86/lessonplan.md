# Reading x86

Now that we have understood what basic C code does and what it does, we can start
to attempt to read some x86 assembly. We will go through how the computer stores
data in the stack, and what are the sort of instructions the computer is able to
execute. Lastly, we will end off by decompiling some assembly and attempt to break
into a password enabled system. 

## Yay, lets begin!

OK so now that you know what is going on in C, let us take a program and try to
reverse the process of compiling it. We are going to take some C code that has been
chewed up by a compiler and converted into a languages your thinking rock can
understand, and try to turn it back into language we can understand.

Your boss managed to find a system which contains sensitive information about your
competitors. He has given you the task of breaking into the [password secured file][password]
See if you can reverse engineer it...

So lets decompile it!

[password]: ./src/password


## What is going on?!

I mean you probably expected that... We've only learnt how to read C anyways. Now
let's go through how to understand this mess.

Remember we mentioned in the previous lesson that the place the main code runs is
called the `main` function? First we need to look for that. This is the place which
the code will run. See the scroll pane on the left (in IDA, the online disassembler, 
as well as in Hopper), try to find `main` and click on it.

Now that you are in there, you'll have realised what a mess this piece of code is.
What is this eax, ebp thing? Why are some things in brackets (`[]`)? What is a
`mov   qword [rbp+var_8], rax`? Perhaps it'll be better if we started by explaining
what those symbols mean.


## Assembly Instructions

A line of assembly code consist of two parts: On the left, it is the operation the
computer is supposed to execuse. These are the stuff you see as `mov` or `push` or
`add` or `call`. On the right, you see the parameters of the instructions. Exactly
what is supposed to be `call`ed or `push`ed, and as well, into what?


## Registers

You might think that a computer pushes data directly into memory. While sometimes
that is true, it is a very slow process. In order to speed things up, computer
designers have created these things called **registers**, which are kinda like
little pieces of memory which allows for quick memory usage. This speeds up the
process of accessing data, and allows 

In the x86 architecture, there exists 8 main registers, however, we will only take
note of 6 of these registers, split into two different groups. Each of these
registers do play special roles, but for this workshop we will see each of the
registers as general purpose registers, and not delve too deep into their specific
functions.


### "Data" based registers: `eax`, `ebx`, `ecx`, `edx`

These 4 registers will form the basis of data handling in x86. You will be using
these registers to manipulate specific pieces of data throughout the program, and
performing operations on them. These registers are large enough to hold an `int`
(that is, they are 32 bits wide).

However, it may also be possible to see these registers represented as `ax`, `bx`,
`cx` and `dx`. These represents the bottom 16 bits. They don't really appear a lot
but just in case the appear, you'll know what is going on. Each of the registers
also has a sort of purpose. For example, `eax` is used as the return value register.
When a function returns a value, it is usually stored in the `eax` register. `ecx`
is usually used as a counter, for loops. More on this in the next chapter.

Suppose I want to work with a single variable. That's fine, we can just take the 
value of the variable and store it in a register. Then we are able to work with it.
At least, that is what you expect the computer to do. However, when we try
compiling the code and seeing the result, we get:

```
int x = 5;   // mov   DWORD PTR [rbp-4], 5
```

Oh no, what is this `DWORD PTR [rbp-4]`? It's time to introduce the stack.


## Assigning variables to the stack.

Suppose we declare 5000 new `int` variables. Where would they go to then? This is 
where the **stack** comes into play. The stack is a place in the computer's memory 
which stores data that gets created in a function. Initialising a new variable would
assign that variable to a certain memory location on the stack.

Code which are supposed to be executed would not be stored in the stack. The only
exceptions are when there seems to be some form of security through obscurity or an
exploit that is being run. Generally think of the stack as a location for data to
exist.

Now, the idea of stack is literally described by its name: It is a stack of data.
In other words, whatever was last put into the stack would be the first one.
Suppose I want to push some `char`s (say `"abcdef"`) onto the stack. Each `char`
would be put onto the stack one by one. So `'a'` goes in first, followed by `'b'`,
and then `'c'`, and so on. The final stack would therefore contain the characters
in reverse order.

So suppose you are reading some strings from the stack, do not be surprised if the
characters of the strings are backwards!

For example in the executable [assignment][assign], if we go to the main section, 
we should see the following pieces of assembly (from IDA pro):

```
mov     [rbp+var_C], 0Fh
mov     [rbp+var_8], 186A0h
mov     [rbp+var_4], 0DABBEDh

mov     [rbp+var_F], 33h
mov     [rbp+var_E], 50h
mov     [rbp+var_D], 6Dh
mov     eax, 0
```

What is the original code that generated this?

```
int x = 15;
int y = 100000;
int z = 14334957;

char a = '3';
char b = 'P';
char c = 'm';

return 0;
```

Note that firstly, all the code has been converted into their hexadecimal
counterparts. Remember that a computer works with binary. When we key in a
number, it gets saved as a decimal number. However, the thing about binary numbers
are that they are very long. Hence, to make it easier for humans to read and
estimate the scale of a number, IDA pro shows the nibbles to us in hexadecimal.

And secondly, there are no more names in the compiled version of the program. This
is the first thing to take note about programming: the names that we give variables
are only to help ourselves identify the variables within the C code that we type.
Once the computer finish compiling the code, each of these variables gets assigned a
memory address on the stack. Think of this as the house the data is in. Whenever the
particular memory address is referenced, we can think of it as referring to that
particular variable that lives within the house. The memory address are the
`[rbp+var_C]` things you see.

In IDA Pro, it gives us the option of renaming the variables. If you are trying to
professionally reverse engineer a program, you should, as much as possible rename
the variables. That will help you trace through the program really easily.

Now that we have introduced the idea of a stack, we can talk about the last two
pointers, one of which you can already see in the memory address on the stack:

[assign]: ./src/1assignment.c

### ebp, esp

These two registers are the __stack base pointer__ and the __stack pointer__
respectively. Why is it required that we have two pointers?

The stack also serves one more purpose: Function calls. These two registers help 
with setting up the stack for a function call. We will go through more about
function calls in the next chapter.

While we are talking about function calls, there are two more registers I have not
covered yet. And those are...

### esi, edi

But for now lets just pretend they are just 2 other registers. We will see them a
bit later, when we try to decode the challenge our boss has given us. 

And with that, I present a picture of all the registers I clearly just screenshotted
from wikipedia:

[References to the registers][registers]

[registers]: ./images/registers.png

 
## Operations in x86

Code in x86 assembly have 2 parts. First the operation itself, this is the
few character names you see on the left most side of each line of code. Second,
there are the parameters. This is the stuff on the right of the operation. The
parameters can be split into two parts: First the destination, this is the stuff
before the comma. Second the source, that is what is after the comma.

Take the instruction `mov     [rbp+var_4], 0DABBEDh` for example. The operation is
`mov`, the destination is `[rbp+var_4]`, and the source is `0DABBEDh`.

There are also other instructions that do not have 2 parameters. These are mainly
the `jump` series of instructions. It will be covered below, but here is one 
example: `mov     [rbp+var_4], 0DABBEDh`

The MOVE (`mov`) instruction simply tells the computer that some data is going to
be *COPIED* from the second parameter into the first parameter. For example if the
instruction says `mov  eax, ebx`, it means whatever data was in register `ebx` will
be copied into the register `eax`. Simple enough.

The ADD (`add`) and SUBTRACT (`sub`) instructions takes two parameters, and modifies
the first parameter using the second parameter. For example, suppose we have the
instruction `add  eax, ebx`, this means `eax` will get the value of `ebx` added to
it. In the same way, `sub  edx, ecx` mean `edx` will have the value of `ecx`
subtracted from it. The same idea works for the AND (`and`), OR (`or`) and XOR
(`xor`) instructions as well.

Bitshifting instructions are about the same, except their codes are abbrevations.
The code for shifting left is `shl` and the code for shifting right is `shr`.
However, sometimes you might also see the code `sal` and `sar` used for bitshifting
as well. They essentially do the same purpose, except this instruction happens
because of the way computer stores numbers.


### Quick digression on how computer stores numbers

Suppose I want to count backwards from 15. We can just say:

15... 14... 13... 12... 11... 10...

But what comes after the symbol `0`? Well that's easy, considering that when we were
counting up, when we hit the symbol `9`, we just reset the symbol to `0`, and added
one to the next digit. So if we are counting *down* instead, we can just reset the
symbol to `9`, and subtract one from the previous symbol!

11... 10... 9... 8... 7... 6... 5... 4... 3... 2... 1... 0...

Uh oh. We have a problem now. While we can say to reset the symbol to `9`, subtract
one from the previous digit and start counting downwards again, what are we supposed
to subtract one from? There are no other previous digits to speak of. The solution 
to this problem is therefore to add the negative sign (`-`) and then start counting
upwards!

0... -1... -2... -3... 

And that works in our decimal system. However, what if happens inside a computer?
There is no negative sign in a computer right? The idea that the we came up with is
to pretend that outside of the range of the number, there exists a non-zero value 
that we can borrow from. Thus after counting down and reaching 0, the number would
jump back up to the highest possible number: A number with all `1`s. In this example
suppose we are counting with a 4 bit number:

```
Binary : 101... 100... 11... 10... 1... 0... 1111... 1110... 1101...
Convert:  5 ...  4 ...  3...  2... 1... 0...  15 ...  14 ...  13 ...
Decimal:  5 ...  4 ...  3...  2... 1... 0...  -1 ...  -2 ...  -3 ...
```

We call this an integer underflow. There is a similar thing that can happen if you
are counting upwards and your numbers get too big. This is called an integer
overflow:

```
Binary : 1100... 1101... 1110... 1111...  0...  1... 10...
Convert:  12 ...  13 ...  14 ...  15 ... 16... 17... 18...
Decimal:  12 ...  13 ...  14 ...  15 ...  0...  1...  2...
```

Wait, so how does the computer know whether a number is supposed to be a negative
number, or a really big positive number? Sadly: the computer doesn't. Hence it is
up to us as programmers of a program to ensure that these situations don't happen.
And this is where `shl` vs `sal` comes in. `shl` simply shifts the bits without
caring for the actual sign and underlying number, while `sal` takes the sign into
consideration.

But I digress.

- - - -

This can be an entire chapter on its own, but we will cover only these basic few.

To summarise:

|  Name   |     Code      |        Remarks       |
|---------|---------------|----------------------|
|   move  |  mov   r1, r2 |   Copies s1 into s2  |
|   add   |  add   r1, r2 |                      |
|   sub   |  sub   r1, r2 |                      |
|   or    |  or    r1, r2 |                      |
|   xor   |  xor   r1, r2 |                      |
|   and   |  and   r1, r2 |                      |
| rshift  |  shr   r1, r2 | Sometimes can be sar |
| lshift  |  shl   r1, r2 | Sometimes can be sal |


## Putting it all together:

Given this [piece of code][bitwise]:

```
int main() {
    int x = 22; //0b10110
    int y = 10; //0b01010

    int PLUS = x + y;
    int MINUS = x - y;
    int AND = x & y;
    int OR = x | y;
    int XOR = x ^ y;
    int LSHIFT = x >> y;
    int RSHIFT = x << y;>

    return 0;
}
```

These becomes:

```
mov     [rbp+var_24], 16h  // This is x (22 = 0x16). x now lives in the house [rbp+var_24]
mov     [rbp+var_20], 0Ah  // This is y (10 = 0x0A). y now lives in the house [rbp+var_20]

mov     eax, [rbp+var_24]  // Copy x into edx
add     eax, [rbp+var_20]  // Add the value of y into eax.
mov     [rbp+var_1C], eax  // Copy the new value of eax into a new house at [rbp+var_1C]

mov     eax, [rbp+var_24]  // Copy x into eax
sub     eax, [rbp+var_20]  // Subtract the value of y from eax
mov     [rbp+var_18], eax  // Copy the new value of eax into a new house at [rbp+var_18]

mov     eax, [rbp+var_24]  // Copy x into eax
and     eax, [rbp+var_20]  // AND the value of y with eax
mov     [rbp+var_14], eax  // Copy the new value of eax into a new house at [rbp+var_14]

mov     eax, [rbp+var_24]  // Copy x into eax
or      eax, [rbp+var_20]  // OR the value of y with eax
mov     [rbp+var_10], eax  // Copy the new value of eax into a new house at [rbp+var_10]

mov     eax, [rbp+var_24]  // Copy x into eax
xor     eax, [rbp+var_20]  // XOR the value of y with eax
mov     [rbp+var_C], eax   // Copy the new value of eax into a new house at [rbp+var_C]

mov     eax, [rbp+var_20]  // Copy y into eax
mov     edx, [rbp+var_24]  // Copy x into edx
mov     ecx, eax           // Copy eax (y) into ecx
sar     edx, cl            // Shift edx rightwards by an amount equal to the lowest 8 bits of cl (y)
mov     eax, edx           // Copy the value of edx into eax
mov     [rbp+var_8], eax   // Copy the new value of eax into a new house at [rbp+var_8]

mov     eax, [rbp+var_20]  // Copy y into eax
mov     edx, [rbp+var_24]  // Copy x into edx
mov     ecx, eax           // Copy eax (y) into ecx
shl     edx, cl            // Shift edx leftwards by an amount equal to the lowest 8 bits of cl (y)
mov     eax, edx           // Copy the value of edx into eax
mov     [rbp+var_4], eax   // Copy the new value of eax into a new house at [rbp+var_4]

mov     eax, 0             // Prepare to return 0.
```

If you are trying to reverse engineer other executable files, you might be lucky
enough to meet other instructions. In that case, you may want to consult online
guides and documentation. More can be found at [this website][x86_instructions]

[bitwise]: ./src/2bitwise.c
[x86_insturctions]: http://kernfunny.org/x86/


## Logic

But what is the point of a computer if it is unable to think for itself? It needs to
be smart, and be able to make decisions based on states which it already knows! This
is where if / else statements come in. We need to be able to let our computer know
how, and when to branch.

The main idea behind if else statements is that you want the computer to do
something should a certain condition be fulfilled. You also want to be able to tell
the computer to do something else should another set of conditions not be fulfilled.
This is done in x86 assembly through the use of the compare instruction (`cmp`) and
the `jump` series of instructions. For now, just take it as

The `jump` series of instructions are usually preceeded by a `cmp` instruction. They
are based on the result of the previous compare instruction, and the jump would be
taken based on their (pretty self explainatory) codes. There are 4 main `jump`
instructions in the jump series. They are as such:

| Code |              Name             |
|------|-------------------------------|
|  je  |          Jump if Equal        |
|  jne |        Jump if not Equal      |
|  jl  |        Jump if Less than      |
|  jlt |   Jump if Less than or Equal  |
|  jg  |      Jump if Greater than     |
|  jge | Jump if Greater than or Equal |
|  jmp |       Unconditional Jump      |


### Not so short digression to how `cmp` works

In a computer, on top of the 8 registers we introduced above, there also exists a
special register called the _flag register_. This register is in charge of taking
note when certain conditions in the code are being fulfilled. An example of a flag is
the overflow flag (`OF`). This flag triggers when the value of the piece of data
we are working with exceeds the number of bits available in the register. An
example would be when we subtract a large negative value from a large
positive value. The resulting value might be bigger than the largest positive
number that could be represented by the number of bits of the register, and hence
would result in an overflow, setting the flag.

Of particular interest is the _zero_ flag (`ZF`). It is set when the results of an
arithmetic operation results in a zero value. The next instruction is usually
followed by a `jz` instruction. This is because the `jz` instruction relies on the
zero flag to decide whether it should jump to the next value.

Another flag we should take note of are the _sign_ flag (`SF`). This lets us know
whether the result of the previous instruction was positive or negative. If the
result is a negative number, then the sign flag would be set to 1. With the
three flags above, we can reason out a couple of how some of the instructions from
the `jump` series would rely on the flags to work.

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
series.

- - - -

## Breaking the admin system?

Now that we know so much, we should be able to break into the admin system! Nope.
Sadly, it's not so easy. We still need to cover one last piece of important
information, and that is function calls. On to the next lesson!
