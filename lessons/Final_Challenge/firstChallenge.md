#Final puzzle (Original)

We are given the following piece of code:

```
rcs:

var_18= dword ptr -18h
var_14= dword ptr -14h
var_4= dword ptr -4

push    rbp
mov     rbp, rsp
mov     [rbp+var_14], edi
mov     [rbp+var_18], esi
mov     [rbp+var_4], 0
jmp     short loc_40066F

loc_400659:
mov     eax, [rbp+var_18]
and     eax, 1
test    eax, eax
jz      short loc_400669

mov     eax, [rbp+var_14]
add     [rbp+var_4], eax

loc_400669:
shl     [rbp+var_14], 1
shr     [rbp+var_18], 1

loc_40066F:
cmp     [rbp+var_18], 0
jnz     short loc_400659

mov     eax, [rbp+var_4]
pop     rbp
retn
rcs endp      "rcs function end


main: 

var_14= dword ptr -14h
var_10= dword ptr -10h
var_C= dword ptr -0Ch
var_8= qword ptr -8

push    rbp
mov     rbp, rsp
sub     rsp, 20h
mov     rax, fs:28h
mov     [rbp+var_8], rax
xor     eax, eax
mov     edi, offset format ; "Enter UserID:"
mov     eax, 0
call    _printf
lea     rax, [rbp+var_14]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
mov     edi, offset aEnterPasscode ; "Enter Passcode:"
mov     eax, 0
call    _printf
lea     rax, [rbp+var_10]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
mov     eax, [rbp+var_14]
cmp     eax, 270Fh
jle     short loc_4006EF

mov     eax, [rbp+var_10]
cmp     eax, 270Fh
jg      short loc_400700

loc_4006EF:
mov     edi, offset s   ; "Numbers are too small."
call    _puts
mov     eax, 0
jmp     short loc_400744

loc_400700:
mov     eax, [rbp+var_10]
mov     edx, eax
mov     eax, [rbp+var_14]
mov     esi, edx
mov     edi, eax
call    rcs
mov     [rbp+var_C], eax
cmp     [rbp+var_C], 6954A9Bh
jnz     short loc_400735

mov     edi, offset aYouWin ; "You win!"
call    _puts
mov     edi, offset aFlag ; "<flag>"
mov     eax, 0
call    _printf
jmp     short loc_40073F

loc_400735:
mov     edi, offset aWrongAnswer ; "Wrong Answer"
call    _puts

loc_40073F:
mov     eax, 0

loc_400744:
mov     rcx, [rbp+var_8]
xor     rcx, fs:28h
jz      short locret_400758

leave
retn

```

Lets reverse it.


## Scanning and printing

Looking at the start, we have:

```
push    rbp
mov     rbp, rsp
sub     rsp, 20h
mov     rax, fs:28h
```

Remember this this bit is just setting up the variables for the `main` function.

```
mov     [rbp+var_8], rax
xor     eax, eax
mov     edi, offset format ; "Enter UserID:"
mov     eax, 0
call    _printf
```

We move the string `"Enter UserID:"` into edi, and then call printf on to it.
This prints out the string "Enter UserID" into the terminal.

```
lea     rax, [rbp+var_14]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
```

Notice that this bit of code all leads to a function call to `scanf`. This lets me
know that the code I'm looking at should be to set up the function call.

Now, we load the effective address of `[rbp+var_14]` into the `rax` register, then we
move that into the `rsi` register. Remember that in a scanf call, the location where
the scanned variable will go to is the memory address that is in the `rsi` register.

We need to determine the format of the the input. Notice that the string `%d` has
been moved into the `edi` register. This lets me know that the function call would
expect a decimal integer being entered.

```
mov     edi, offset aEnterPasscode ; "Enter Passcode:"
mov     eax, 0
call    _printf
lea     rax, [rbp+var_10]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
```

Now the same can be said for this piece of code: We move the string `"Enter Passcode:"`
into the `edi` register, and then call printf. Then we move the address of `[rbp+var_10]`
into the `rsi` register, put the `%d` format string into the `edi` register, and then
call scanf. This just lets me know that both the UserID and the Passcode are supposed
to be integers.

## Some checks?

We then come to some simple checks on the code:

```
mov     eax, [rbp+var_14]
cmp     eax, 270Fh
jle     short loc_4006EF
```

Remember that the variable at `[rbp+var_14]` is the value of the UserID. We are
comparing it to the value `270Fh = 9999`, and jumping if it is less than or equal
to the value. Notice that it will jump to the location `loc_4006EF`. The code for
this is here:

```
loc_4006EF:
mov     edi, offset s   ; "Numbers are too small."
call    _puts
mov     eax, 0
jmp     short loc_400744

loc_400744:
mov     rcx, [rbp+var_8]
xor     rcx, fs:28h
jz      short locret_400758

leave
retn
```

Oh, it prints the string `"Numbers are too small"` and then jumps to `loc_400744`,
which essentially leaves the `main` function. Oh no, that's not what we want!
We can therefore conclude that we want that bit of code *not* to jump! Hence, we
conclude that the first number that we input must be greater than 9999!

Suppose we input a number that is greater than 9999, then we will move to this next
bit of code:

```
mov     eax, [rbp+var_10]
cmp     eax, 270Fh
jg      short loc_400700
```

Now, we are moving the value of `[rbp+var_10]` into the `eax` register, and then
comparing it with the value `270Fh = 9999`. We will jump to the location `loc_400700`
if the value of `eax` is greater than `9999`. Notice that this jump will take us away
from the piece of code that puts `"Numbers are too small"`. Therefore we conclude 
that we would want to take the jump. From here, we can therefore deduce that both
numbers that we input has to be bigger than `10000`. Good job, onwards to the next
section.

## Preparing for the function call!

Now we have this code:

```
loc_400700:
mov     eax, [rbp+var_10]
mov     edx, eax
```

We move the value of `[rbp+var_10]` into the `eax` register, and then move that
into the `edx` register. Remember that the value of `[rbp+var_10]` contains the
value that we supplied for the passcode.

```
mov     eax, [rbp+var_14]
mov     esi, edx
mov     edi, eax
```

Then we move the value of `[rbp+var_14]` into the `eax` register. Remember that this
value is the value that we put for the UserID field.

OK, we then move the value for `edx` into the `esi` register, and the value of `eax`
into the `edi` register. Remember that when `esi` and `edi` shows up, we will be
preparing to call a function. The parameters for the functions are stored in those
two variables! Lets see what function gets called:

```
call    rcs
```

Right. We didn't give you the `rcs` function for nothing eh.

So the thing we now need to figure out is what this function actaually does. Is it
supposed to return something? Remember that if it returns something, it would be 
stored in the `eax` register. Lets look at the code directly after the function to
see if `eax` is being called somewhere:

```
mov     [rbp+var_C], eax
cmp     [rbp+var_C], 6954A9Bh
jnz     short loc_400735
```

Right! We move the value of `eax` into the stack at `[rbp+var_C]` and then compare
that with the value `6954A9Bh = 110447259`. It will then jump to a certain location
in memery if the answer is not the same.

So it seems we need to figure out what the function `rcs` returns! Let's do that.

## `rcs` function call!

Fun fact: rcs actually stands for "recurse". Originally I had planned to give a
recursive function for y'alls to reverse, but that's far too sadistic haha.

Now lets have a look at what `rcs` does:

```
push    rbp
mov     rbp, rsp
```

Setting up the stack frame of the function.

```
mov     [rbp+var_14], edi
mov     [rbp+var_18], esi
mov     [rbp+var_4], 0
jmp     short loc_40066F
```

We move the value of `edi` (which currently contains the value of the UserID) into
the stack at `[rbp+var_14]`. We then move the value of `esi` (which contains the
value of passcode) into the stack at `[rbp+var_18]`. We conclude that `[rbp+var_14]`
contains the UserID, and `[rbp+var_18]` contains the passcode.

We then jump to the location for `loc_40066F`.

```
loc_40066F:
cmp     [rbp+var_18], 0
jnz     short loc_400659
```

It compares if the value at `[rbp+var_18]` is equal to `0`. If it is still not equal
to `0`, it will jump to `loc_400659`. Remember that since at the start, this value
is whatever we have input for the passcode, it will definitely not be equal to 0
(unless we purposely typed in 0 as our passcode...). Lets follow the jump for now:

```
loc_400659:
mov     eax, [rbp+var_18]
and     eax, 1
test    eax, eax
jz      short loc_400669
```

This bit seems to be moving the value of `[rbp+var_18]` into the `eax` register.
Then it is doing bitwise `and` of `eax` and `1`, and checking if that is equal to
`0`. If it is equal to 0, it skips the next section of code.

Lets think about this for a moment: What numbers when called a bitwise `and` with
`1` will result in a non-zero value? Well, lets put a random binary number into the
equation and try it out for ourselves:

|  Value of  | 2\*\*2 | 2\*\*1 | 2\*\*0 |
|------------|--------|--------|--------|
|   Number   |   ?    |    ?   |    x   |
|      1     |   0    |    0   |    1   |
| Number & 1 |   0    |    0   |    x   |

So from this table, we realise that the result of `Number & 1` is not equal to
zero is when the smallest bit of the number is 1. And note that this just means
that the number in question is odd! We will only jump into that section of code
when `[rbp+var_18]` is odd. So what happens in that section of code?

```
mov     eax, [rbp+var_14]
add     [rbp+var_4], eax
```

Aha! It moves `[rbp+var_14]` into `eax` and adds that into `[rbp+var_4]`! So it
seems that the variable `[rbp+var_14]` is a counter! Immediately after this, both
paths converge into this code:

```
shl     [rbp+var_14], 1
shr     [rbp+var_18], 1
```

We shift the variable in `[rbp+var_14]` to the left by 1 position, and we shift the
value of the variable in `[rbp+var_18]` to the right by 1. This simply means we 
multiply the value of `[rbp+var_14]` (which in this case is UserID) by 2, and divide
the value of `[rbp+var_18]` (which is passcode by 2. This code then goes back to
the original check to see if passcode is equal to 0.

```
loc_40066F:
cmp     [rbp+var_18], 0
jnz     short loc_400659
```

Suppose for a moment that passcode is equal to 0, then we will go to this bit of
code:

```
mov     eax, [rbp+var_4]
pop     rbp
retn
rcs endp      "rcs function end
```

The value of `[rbp+var_4]` (our counter) get moved into `eax` register and returned
as the return value from the `rcs` function. OK now that we know how the computer
will execute the `rcs` function, we can try plugging in some numbers and seeing how
the value of `rcs` changes.

## Simulating rcs by hand

```
push    rbp
mov     rbp, rsp
mov     [rbp+var_14], edi
mov     [rbp+var_18], esi
mov     [rbp+var_4], 0
jmp     short loc_40066F

loc_400659:
mov     eax, [rbp+var_18]
and     eax, 1
test    eax, eax
jz      short loc_400669

mov     eax, [rbp+var_14]
add     [rbp+var_4], eax

loc_400669:
shl     [rbp+var_14], 1
shr     [rbp+var_18], 1

loc_40066F:
cmp     [rbp+var_18], 0
jnz     short loc_400659

mov     eax, [rbp+var_4]
pop     rbp
retn
rcs endp      "rcs function end
```

Suppose we entered `9` and `5` as our UserID and passcode respectively. Then when
we hit the `jmp` instruction to jump to `loc_40066F`, we have the following values
in our stack and registers:

|    Address     | Value |
|----------------|-------|
| `[rbp+var_4]`  |   0   |
| `[rbp+var_14]` |   9   |
| `[rbp+var_18]` |   5   |
|                |       |
|     `eax`      |   ?   |

We jump to `loc_40066F`. There, if `[rbp+var_18]` is not equal to `0` there will be a
jump backwards. Well we can see that `[rbp+var_18]` is clearly `5`, so it will jump
backwards. Lets follow that jump.

The next bit checks if `[rbp+var_18]` is odd. Since `5` is odd, we will not take the
jump. In this case, we move the value in `[rbp+var_14]` into `eax` register, and then
add it to `[rbp+var_4]`. After this step we have this:

|    Address     | Value |
|----------------|-------|
| `[rbp+var_4]`  |   9   |
| `[rbp+var_14]` |   9   |
| `[rbp+var_18]` |   5   |
|                |       |
|     `eax`      |   9   |

Now, we shift the value in `[rbp+var_14]` to the left by one position, effectively
dividing it by 2, and we shift the value in `[rbp+var_18]` to the right by one
position, effectively multiplying the value by 2. Then we have this:

|    Address     | Value |
|----------------|-------|
| `[rbp+var_4]`  |   9   |
| `[rbp+var_14]` |   18  |
| `[rbp+var_18]` |   2   |
|                |       |
|     `eax`      |   ?   |

Now we are back where we started: At the check. `[rbp+var_18]` is not `0`, so we
go through the loop again. This time however, since `[rbp+var_18]` is not odd,
we do not shift anything into `eax` and therefore nothing gets added to the value
at `[rbp+var_4]`. At the end of the second loop, the values look like this:

|    Address     | Value |
|----------------|-------|
| `[rbp+var_4]`  |   9   |
| `[rbp+var_14]` |   36  |
| `[rbp+var_18]` |   1   |

Once again, the value of `[rbp+var_18]` is still not `0` yet. Therefore we run
through the loop again. This time however, `[rbp+var_18]` is odd! Hence we will
need to add the value of `[rbp+var_14]` to `[rbp+var_4]`. Then after that loop,
the values of each of the stack variables:

|    Address     | Value |
|----------------|-------|
| `[rbp+var_4]`  |   45  |
| `[rbp+var_14]` |   72  |
| `[rbp+var_18]` |   0   |

Finally `[rbp+var_18]` is equal to `0`, we can therefore go stop looping. We then
shift the value of `[rbp+var_4]` into the `eax` register and return it. At this
stage, the `rcs` function has ended.

Wait a minute, when we key in `9` and `5`, the number that is returned is `45`!
Could it be that `rcs` function is really just multiplying the two input numbers
together? Yes it is! Why it does so can be found [at this link][karatsuba].

[karatsuba]: https://www.cut-the-knot.org/Curriculum/Algebra/EgyptianMultiplication.shtml

## Putting it together

Right, so now that we know what each of this does, lets try to reconstruct the
source code for this puzzle. First it scans in two numbers as the "UserID" and 
"passcode". It then checks these two numbers to see if they are greater than `10000`.
If they are, the two numbers gets multiplied together (via the function `rcs`) and
the result gets checked against the number `110447259`.

### Alright what is the answer?

We need to find two numbers that are both above 10000 that when multiplied together
gives the number `110447259`. If we try to factorise `110447259`, we get that it has
factors: `3 * 13 * 283 * 10007`. Now suppose we choose 10007 as the UserID, then the
passcode has to be `110447259 / 10007 = 11037`. And this passcode fits the
constraints! Therefore we key in the value `10007` for the UserID, and the value
`11037` as the password.

```
>>> Enter UserID: 10007
>>> Enter Passcode: 11037
>>> You win!
>>> flag{RevEngeSprout2018}
```

Congratulations. You have solved the final puzzle.
