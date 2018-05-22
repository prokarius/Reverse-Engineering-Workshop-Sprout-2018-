# Functions

Now that we have the basics of x86 assembly down, we need to cover the harder stuff.
The stuff that makes up the core of computing: Functions. How does the program know
what functions to call, and what are the assembly instructions for that function?


## Calling Functions

Remember in the intro, we mentioned the entry point into a program is the main()
function? But aside from that, there could be other functions that are around too.
Users may define various other functions which would help the source code look
neater, or help to abstract away various piece of code that will be repeatedly
called.

To declare a function in source code, we mentioned that we will need 3 things:
The name of the function, the return type of the function, as well as the types
and names of input parameters into the function. Suppose I want to define a
function that takes two numbers and adds them together. The function will return.
The `char` 'F' if the sum of the two values is less than 50, and the `char` 'P' if
the sum of the two values is greater than or equal to 50.

This function is kinda like a grading system, so perhaps we will call the function
`grader`. The data that is returned has a type of `char`, and the two inputs have
types `int`s. Giving the inputs smarter names we might have coded 
[something like this][grader]:

```
1   char grader(int test, int exam){
2       int total = test + exam;
3       if (total >= 50){
4           return 'P';
5       }
6       else {
7           return 'F';
8       }
9   }
10
11  int main(){
12      int testscore = 26;
13      int examscore = 22;
14      char grade = grader(testscore, examscore);
15
16      return 0;
17  }
```

I have included the line numbers on the left of the program for easy reference.
Let's think about what actually goes on when we run this program.

[grader]: ./src/1grader.c


## Analysis Time (Incomplete version)

First, we will enter the `main` function at line `11`. Moving to line `12` and `13`,
we declare two new `int` variables `testscore` and `examscore`. Because these are
new variables within the `main` function, they would be pushed into the stack, each
with its own memory address. So far so good. Oh, and mind the crappy table style
drawing of the stack. I was too lazy to go screen shot the stuff properly.

|         Stack        |
|----------------------|
|                      |
|                      |
| (int) examscore = 22 |
| (int) testscore = 26 |

Now, in line `14`, we run the code `char grade = grader(testscore, examscore);`.
A lot of things are going on in this single line, so let's think about what the
computer has to do to execute this line of code: First, it needs to figure out what
result the function `grader` would return when given the two arguments `testscore`
and `examscore`. These two arguements were defined in line `12` and `13`, so by the
time we reach here, they can be seen as numbers.

Now, after passing the two numbers into the function, the computer needs to go into
the function `grader` and work out what the result will be. Now according to line
`1`, the two numbers that gets passed into the function are not called `testscore`
and `examscore` anymore, but rather, they now have the name `test` and `exam`.

Side note: remember in line 14, we know that the variable `testscore` has value
26, and the variable `examscore` has value 22. Hence, line `14` can essentially be
seen as doing: `char grade = grader (26, 22);`. So just like in algebra, we refer to
line `1`: The variable `test` therefore gets the value 26, and the variable `exam`
therefore gets the value 22. 

Moving on, the computer now goes to line `2`, which requires it to make a new 
variable on the stack called `total`. So now on the stack we have another variable
called total, that gets the value `26 + 22 = 48`.

|         Stack        |
|----------------------|
| (int) total = 48     |
| (int) exam = 22      |
| (int) test = 26      |
| (int) examscore = 22 |
| (int) testscore = 26 |


Now, we are at line `3`: `if (total >= 50)`. The computer asks itself if the
variable called `total` has a value greater than or equal to 50. Well from what we
have calculated it isn't, so instead of jumping to line `4` and returning the
`char` `'P'`, we jump to line `6` and return the `char` `'F'`. Hooray! We have
finished analysing the function `grader`. Lets go back to our previous function.

Though... we have a problem. What is our previous function? We didn't really save
a copy of which line we were supposed to go back to, nor does my computer ever know!
This is when we need to explain the two most important registers when it comes to
stack management: `ebp` and `esp`. At the same time, we will take a quick mention
about the instruction pointer as well: `pc`.


## Stack Management with `ebp` and `esp`

So the problem was how the program knows which variables belong to which function,
as well as which line the program is supposed to jump back to once it is finished
running a particular function. This is done with the two registers `ebp` and `esp`.

The main idea is that each function that gets called gets it's own region in the
stack frame. Think of it as an avenue where all the houses with variables live in.
Once the function is no longer needed, that particular avenue is bulldozed to make
space for future function calls.

During compilation, the compiler will figure out how much space is necessary for
each particular function. That will be the amount of space in the memory that the
function gets. The space in which all the variables of a function lie is called the
function *frame*. Remember that since the stack grows *downwards*, the base of the
stack should be at a higher memory address than the top of the stack.

Hence in order to define where the stack is, we first set the base pointer at the
top of the previous frame. Then we need to calculate the amount of space the
function requires. This is then subtracted from the base pointer and set to be the
pointer at the top of the stack frame.

Suppose a certain function `fn` requires `40 = 0x32` bytes of memory. Then, after
setting up the stack frame the contents might look a little like this:

|  Address Offset   |  Stack Contents   |        Remarks        |
|-------------------|-------------------|-----------------------|
|       -0x32       |                   |   `rsp` points here   |
|                   |                   |                       |
|                   |                   |                       |
|       -0x00       |                   |   `rbp` points here   |


That solves the problem of which variables belong to which function. But the next
problem is where do we jump back to? How do we know which line of code we are
supposed to return to once we are done with the execution?


### Instruction Execution with `pc`

The program counter (`pc`) is a register which stores the address of the current
instruction being executed. It is similar to your finger when you are manually
tracing code. However, in a computer, it does not have fingers. It however has
registers, and hence, stores the code which it is supposed to run in this dedicated
register called the `pc` register. 

Now the problem is when you are jump to another piece of code, once you are done
executing that bit of code, you might have forgotten where you came from. In that
case, the solution is to save the location of where you will backtrack to on the
stack. Then when you are done with the function that was called, you can simply
return to where you were with the correct return value of the function, and continue
running the program. The following two assembly instructions help with that:


### `call` and `retn` assembly instruction

Very obviously, the assembly instruction `call` is the same instruction to call a
function. It takes the current value of `pc` and pushes it into the stack at the
location of `rsp`. This is then followed by an unconditional jump to the memory
location of the called function.

Similarly, the `retn` function pops a value from the stack, and sets that to be the
`pc`. Remember that since `pc` is the location of the next address the computer is
supposed to run, the next instruction after the `pop` instuction would be the value
that was just popped.

Now, `call` and `retn` instructions should always come in pairs, because when you
make a function call, the only way you will know where to return to is if you used
the `call` instruction to push the current value of `pc` onto the stack, thereby
saving it. When you are done with the function, after collapsing the stack, you can
simply use the `retn` instruction to get back to where you came from.


## Function parameters and returns

Now that we know how a function can be called, we need to talk about functional
parameters. We mentioned before that functions can take in parameters and use them
during their computation. How a computer allows for functions to take in parameters
is to use the registers to pass data around. Specifically, there are two more 
registers that were not mentioned yet.


### `edi` and `esi` registers.

The `edi` and `esi` registers are two registers are are usually reserved for
supplying arguments into functions. Before the function call, they will be loaded
with the value of the arguments that will be passed into the function, then the
`call` instruction will be run, and the computer will be inside the function. Since
the value of the registers do not change across `call` instructions, the value of
the registers will remain as the values of the parameters that were passed into the
function.

By convention, when dealing with functions that take in a number of arguments, the
following registers will be used for each argument in order. Outside of 4 arguments,
there are 2 more registers (the `r9d` and the `r8d`) register which can be used
should the function take in more than 4 variables:

| Parameter | Reg |
|-----------|-----|
|    1st    |`edi`|
|    2nd    |`esi`|
|    3rd    |`edx`|
|    4th    |`ecx`|
|    5th    |`r8x`|
|    6th    |`r9x`|

If more arguments are still necessary, then they would be pushed directly onto the
stack before the `call` instruction gets run. They can still be accessed within the
function, but technically are *outside of the stack pointer* of the function. They
are still considered part of the stack frame, because the stack frame includes all
variables passed into the function as well. (Note that in such a case, clean up
steps are required after the function has ended. This would eat up a couple of
instructions, and make the assembly code generally messier.)


### Returning a value

Now, when a function returns, where would the return value go? Well notice that we
mentioned in the previous lesson that the `eax` register gets the return value of a
function. Before the function ends, you will see some lines of assembly code pushing
a certain value to the `eax` register. That is the return value!


## Analysis Time (Complete ver)

Now that we know all the specifics of function calls, we can try to reverse the
grader program to see how each one of these work together. However, in the interest
of time, this will be skipped in the workshop.

For a more in depth look into the specifics of what gets called in this particular
program, I have written a [seperate document][analysis] that talks in far more
detail about how the grader program works, and what every single chunk of assembly
does.

[analysis]: ./graderdetailed.md


## Input and Output

Anyways, what good does a program do if it can't tell the results of the
computation? There has to be a way for a computer to tell the user by printing out
messages on to the screen. Also, if there is no way for the user to let the computer
know what has to be calculated, then it makes no sense for the computer to compile
the program every single time a new input value need to be run.

Enter the input and output functionality of a computer program. In C, these are
handled by two special functions: `scanf` and `printf`. The parameters into the
`scanf` function is the format string of what needs to be scanned, as well as the 
optional memory address(es) to store what was being scanned. 

A *format string* is a string that lets the computer know what it is the input it is
supposed to be looking out for. The most important part of the format string is the
*formatting flags* that are present within them. These are the `%d` or `%s` or `%c`
things you see within a format string, they will tell you what data type your
program would be looking out for in the input. There 3 format strings above are the
three main ones we will be meeting today:

| Format Flag | Program will read input as... |
|-------------|-------------------------------|
|     `%d`    | -2\*\*31 <= `int` < 2 \*\* 31 | >
|     `%s`    |            A string           |
|     `%c`    |            A `char`           |

Suppose I want to scan in a string, and then two decimal numbers, all of these are
seperated by spaces. Then my format string would look like `%s %d %d`.

The format strings (or more specifically, the memory address of the format strings)
will always be moved into the `edi` register before the function call. Suppose we
are going to scan something into some address. Then the address(es) where the 
input data would be scanned into is given by whatever was passed into the `esi`
register before the `scanf` function call. 

Suppose we are scanning two `int`s. Then the address that will be
passed into the `esi` register would be the address where the first `int` is going
to be stored. When the `scanf` function runs, it will take whatever was input, and
load it onto the addresses that was given.

But wait, how do we know where the address of the first `int` is going to be at?
This is where the `lea` instruction comes in. It essentially Loads the Effective
Address of a given piece of data into a certain register. You would usually see
these pieces of assembly code before a `call` instruction to the `printf` or
`scanf` function.

Now, for the `printf` function, it follows the same idea. Whatever is to be printed
will be loaded into the `edi` register. Then the `printf` function will be called.

Now most of the time, you will see the functions `printf` and `scanf` being called.
However sometimes, the compiler will use the even more basic function `_puts`
(short for *put s*tring) for printing text. It might also sometimes call `_fgets`
(short for formatted get string) for acquiring input as well, just take note of
these.


## Breaking into a crappy admin system

Now that we know all there is to know about very basic reverse engineering, it's
time for your first real task! Remember that piece of code you were unable
to figure out from the first part? Lets try to break it this time. I will break down
the code into chunks and attempt to explain what each chunk does. You can try to
reverse engineer the code without continuing below, if you want to test yourself.

```
push    rbp
mov     rbp, rsp
sub     rsp, 0FC0h
```

As mentioned above, in here we are setting up the stack pointers for the `main`
function call. From here, note that by first moving `rsp` into rbp, and then 
subtracting the value `0xFC0` from it, the program gives `0xFC0 = 4032` bytes of
memory for the stack frame of this function.

```
mov     rax, fs:28h
mov     [rbp+var_8], rax
xor     eax, eax
```

In here, we move the value of `rax` somewhere. Not that it is important though.
The reason why the computer does this is to select some thread specific, OS-related
registers. This makes 

Then by XORing `eax` with itself, it essentially clears it. So now `eax` is empty.

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
register) is `%d`. This tells me the computer expects an integer as the password.
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

### Short note on string compare:

Given two strings (a bunch of `char`s that ends with the `null byte`), it
compares the two strings and returns `0` if the two strings are equal. It returns
an `int` less than `0` if the first string is lexicographically smaller than the
second string, and an `int` greater than 0 is the first string is lexicographically
larger than the second string.

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

Which prints out `YOU ARE NOT THE ADMIN!` and quits the program. (Notice that the
code at memory location `loc4007AF` leads to the end of the main function. Clearly,
we would need to jump to the location `loc_400746`. Hence we want the result of the
`_strcmp` function to be 0. That is, we want the two strings we input in the
function `_strcmp` to contain the same bunch of `char`s in the same order.

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
