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

Now how does a computer do operations? It is unable to 

In the x86 architecture, we have a couple of registers to take note of. 



## Assigning variables

Suppose we declare 5000 new `int` variables. Where would they go to then? Certainly
there would not be enough registers to hold them. This is where the **stack** comes
into play.

The stack is a place in the computer's memory which stores data. Most of the time,
code which are supposed to be executed would not be stored in the stack. The only
exceptions are when there seems to be some form of security through obscurity or an
exploit that is being run.

The computer is able to make use of the stack as a location to store

For example in the code in assignement, if we go to the main section, we should see
the following pieces of assembly. We ignore the rest of the assembly for now,
the reason why those other pieces of assembly code exists is beyond the scope of
these 2.5 hours.

```
mov     [rbp+var_C], 0Fh
mov     [rbp+var_8], 186A0h
mov     [rbp+var_4], 0DABBEDh
mov     eax, 0
```

What is the original code that generated this?

```
int x = 15;
int y = 100000;
int z = 14334957;
```

Note that firstly, all the code has been converted into their hexadecimal
counterparts. And secondly, there are no more names in the program.


## Operations in x86

This can be an entire chapter on its own, but we will cover only the basic few.

### Moving data

The move (`mov`) instruction simply tells the computer that t


If you are trying to reverse engineer other executable files, you might be lucky
enough to meet other instructions. In that case


## Input and Output




## Combining it together




## Breaking into a crappy admin system

So it's time for your first real task! Remember that piece of code you were unable
to figure out from the first part? Lets try to break it this time.





