# Functions

Now that 


## Input and Output

Anyways, what good does a program do if it can't tell the results of the
computation? There has to be a way for a computer to tell 

Enter the input and output functionality. We will deal with input




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
