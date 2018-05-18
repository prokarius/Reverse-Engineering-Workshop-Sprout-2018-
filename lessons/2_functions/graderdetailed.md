## Assembly Time

Now that we know what is supposed to happen when we run this program, we can head
over into the assemble to start to see the machine code in action. Note that when
we decompile the binary, we can look for the grader and the main function on the
left scroll bar. Lets get to reversing:

```
function grader:
push    rbp
mov     rbp, rsp
mov     [rbp+var_14], edi
mov     [rbp+var_18], esi
mov     edx, [rbp+var_14]
mov     eax, [rbp+var_18]
add     eax, edx
mov     [rbp+var_4], eax
cmp     [rbp+var_4], 31h
jle     short loc_4004F8

mov     eax, 50h
jmp     short loc_4004FD

loc_4004F8:
mov     eax, 46h

loc_4004FD:
pop     rbp
retn
add endp


function main:
push    rbp
mov     rbp, rsp
sub     rsp, 10h
mov     [rbp+var_8], 1Ah
mov     [rbp+var_4], 16h
mov     edx, [rbp+var_4]
mov     eax, [rbp+var_8]
mov     esi, edx
mov     edi, eax
call    grader
mov     [rbp+var_9], al
mov     eax, 0
leave
retn
main endp

```

Let us go through one by one what this function does.

## Walking through main

At the start of main, we see two lines of assembly code. These two lines will always
be present at the top of every function call in x86 assemble. This is called the
function preamble, it is there to set up the stack, as we have described in the
previous section above.

### Setting up the stack frame

In the previous lesson we covered the use of the `rbp` and `rsp` registers. Note
that they have made their appearance again at the top of the assembly code again.

```
push    rbp
mov     rbp, rsp
sub     rsp, 10h
```

First, the value of `rbp` (which is the value of the stack base pointer) gets
pushed onto the stack itself. When a value gets pushed onto the stack, its value
gets saved to where `rsp` is pointing. Then `rsp` gets decresed by an amount the
size of the variable.

Suppose `rsp` has a value of `0x38`, and we call `push (int)0x00`. Then into
memory address at `0x38`, we push the value `0x38`. Since the value is an `int`,
therefore we will subtract 4 (because an `int` is 4 bytes large) from `rsp`. In the
end, `rsp` has a value of `0x34`.

Going back to the assembly, the value of `rsp` is then copied into `rbp`. This is
done so that we know where the bottom of the current frame is. The compiler has 
also calculated that the `main` function requires `0x10 = 16` bytes worth of memory
to store all its variables, so it subtracts `0x10` from `rsp`.

The stack now looks like this:

|  Address Offset   |  Stack Contents   |                Remarks                |
|-------------------|-------------------|---------------------------------------|  
|                   |                   |                                       |
|       -0x14       |                   |  (`main`'s end) `rsp` points here     |
|                   |                   |                                       |
|                   |                   |                                       |
|       -0x04       |                   | (`main`'s start) `rbp` points here    |
|  Absolute Bottom  |   `global rbp`    |        Meaningless value to us        |


### Initialising the variables:

Next we have these two lines of instructions:

```
mov     [rbp+var_8], 1Ah
mov     [rbp+var_4], 16h
```

This is simple, it just tells the computer to move the value `0x1A = 26` into the
memory address at `[rbp+var_4]` (which you can easily figure out is













## Entering function `grader`

```
mov     [rbp+var_14], edi
mov     [rbp+var_18], esi
```

Here we can see two variables being moved to two different houses on the stack.
Tracing the size of the pointers (or you can see the top, where it says
`var_18= dword ptr -18h`, you can figure out that the data located at `[rbp+var_18]`
is an `int`. The same can be said for

### Digression into



### Collapsing the stack frame


