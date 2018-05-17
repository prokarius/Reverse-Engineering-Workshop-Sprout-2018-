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

Oh no, what is this DWORD PTR [rbp-4]? It's time to introduce the stack.

## Assigning variables to the stack.

Suppose we declare 5000 new `int` variables. Where would they go to then? This is 
where the **stack** comes into play. The stack is a place in the computer's memory 
which stores data that gets created in a function. Initialising a new variable would
assign that variable to a certain memory location on the stack.

Code which are supposed to be executed would not be stored in the stack. The only
exceptions are when there seems to be some form of security through obscurity or an
exploit that is being run. Generally think of the stack as a location for data to
exist.

For example in the executable [assignment][assign], if we go to the main section, we should see
the following pieces of assembly (from IDA pro):

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
[rbp+var_C] things you see.

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
with that. We will go through more about function calls in the next chapter.

 
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
There is no way 





But I digress

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

Given this piece of code:

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

[x86_insturctions]: http://kernfunny.org/x86/


## Input and Output






## Breaking into a crappy admin system

So it's time for your first real task! Remember that piece of code you were unable
to figure out from the first part? Lets try to break it this time. I will break down
the code into chunks and attempt to explain what each chunk does. You can try to
reverse engineer the code without continuing below, if you want to test yourself.

```
push    rbp
mov     rbp, rsp
sub     rsp, 0FC0h
mov     rax, fs:28h
mov     [rbp+var_8], rax
xor     eax, eax
```

In here, we save the value of rax somewhere. Not that it is important though.
Then by XORing eax with itself, it essentially clears it. So now EAX is empty.

```
mov     rax, 6E696D6461h
mov     qword ptr [rbp+s2], rax
```

We load a magic number into rax, then into some location on the stack [rbp+s2].

```
lea     rdx, [rbp+var_7D8]
mov     eax, 0
mov     ecx, 0F9h
mov     rdi, rdx
rep stosq
```

Some magic it doesn't concern me too much...

```
mov     edi, offset format ; "Enter your username: "
mov     eax, 0
call    _printf
```

Now it loads a string into `edi` (which remember, is the argument for a function).
Then it calls the `printf` function, which means we will see some output. In this
case, the program will print the text `Enter your username: `.

```
lea     rax, [rbp+s1]
mov     rsi, rax
mov     edi, offset aS  ; "%s"
mov     eax, 0
call    ___isoc99_scanf
```

We then shift the address of `[rbp+s1]` into `rsi`. Then we also move the format string
`%s` into `edi`, which tells us the computer expects a string. The program then calls
the function `___isoc99_scanf`. Whatever string was input would be stored in
the memory address `[rbp+s1]`.

```
mov     edi, offset aWhatIsYourPass ; "What is your password: "
mov     eax, 0
call    _printf
```

Same as before, it loads the string `What is your password` and prints it.

```
lea     rax, [rbp+var_FB4]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
```

And now notice that the format string of the program (that was moved into the `edi`
register) is `%d%`. This tells me the computer expects an integer as the password.
This password would then be stored at the house `[rbp+var_FB4]` in the stack.

```
lea     rdx, [rbp+s2]
lea     rax, [rbp+s1]
mov     rsi, rdx        ; s2
mov     rdi, rax        ; s1
call    _strcmp
```

Finally it loads the address of `[rbp+s2]` into `rsi`, and the address of
`[rbp+s1]` into `rdi`. These are the two arguments into the function we are calling:
`_strcmp`. String compare.

```
test    eax, eax
jz      short loc_400746
```

Then, once we are done, we test the output of `eax`. If it is 0, we will jump to the
memory location `loc_400746`. If not, the program will conitnue and execute:

```
mov     edi, offset s   ; "YOU ARE NOT THE ADMIN!"
call    _puts
mov     eax, 0
jmp     short loc_4007AF
```

Which print out `YOU ARE NOT THE ADMIN!` and quits the program. (Notice that the
code at memory location `loc4007AF` leads to the end of the main function. Clearly,
we would need to jump to the location `loc_400746`. Hence we want the result of the
`_strcmp` function to be 0. 

Notice that we are comparing the values in memory location `[rbp+s1]` and `[rbp+s2]`.
Wait a minute, we scanned in a string into the memory location `[rbp+s1]` and at the
start of the program, some magic numbers were put into `[rbp+s2]`. That means if the
string we input when we are asked for our user name matches the magic numbers, we
would be able to continue towards `loc_400746`.

Remember that a string is just a bunch of characters? So what exactly are the
magic characters that were put into `[rbp+s2]`? We need to pick `6E696D6461` apart
byte by byte. Fortunately that isn't too difficult:

| 6E | 69 | 6D | 64 | 61 |
|----|----|----|----|----|
| n  | i  | m  | d  | a  |

Also remember how we mentioned that the stack stores the values of characters
backwars (as it is a stack)? Hence we can deduce that we are supposed to input the
string "admin" as our username.

- - - -

Good job, we have reverse one part of the password program. The next bit is to find
out the correct password. Lets continue reversing from `loc_400746`.

```
loc_400746
mov     eax, [rbp+var_FB4]
sub     eax, 0Eh
mov     [rbp+var_FB4], eax
```

We take the data that lies at house `[rbp+var_FB4]` on the stack, and subtract 0xE
from it. But wait, isn't the data that exists at `[rbp+var_FB4]` the integer
password we input? Converting it, we realise we are subtracting the value 14 from
our input password, and restoring it.

```
mov     eax, [rbp+var_FB4]
shl     eax, 2
mov     [rbp+var_FB4], eax
```

Then we are taking the same password, and shifting it left by 2.

```
mov     eax, [rbp+var_FB4]
add     eax, 9
mov     [rbp+var_FB4], eax
```

Lastly, we are simply adding 9 to the password.

```
mov     eax, [rbp+var_FB4]
cmp     eax, 0ADh
jz      short loc_400791
```

Ah, we are comparing the final modified password with the value `0xAD`. We try to
convert it back into decimal: `10 \* 16 + 13 = 173`.

It's time for some algebra: Suppose we input some password. Call the number `pass`.
First we subtract the number `14` from the password, then we shift it to the left 2
times, then we add `9` to it, and we want to get the number `173`. What should our
original value be?

First we need to undo the addition, so we subtract `9` from `173` to get `164`.
Then we need to shift it to the right by 2 times to undo the left shifts, so we
write `164` in binary:

`164 = 0b10100100`, and we shift it to the right 2 times to get:
`0b101001 = 41`.

Lastly, we need to undo the first subtraction. SO we need to add `14` to `41` to
reveal that the password is supposed to 55 in order for the `jz` instruction to
be taken.

BUT do we want to `jz` instruction to be taken? Notice that if we do not take the
`jz` instruction, we will go to some piece of code that says `WRONG PASSWORD` and
exit the program. Hence we do want the `jz` instruction to be taken!

From this exercise, we deduce that when prompted for the username, we should type
`admin`, and when prompted for the password, we should type `55`. Lets try it.

```
Enter your username: admin
What is your password: 55
THANK YOU ADMIN!
BUT OUR PRINCESS IS IN ANOTHER CASTLE!
```

What!? This isn't Mario!
