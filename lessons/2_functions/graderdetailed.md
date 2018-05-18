# Grader Function

Now that we know what is supposed to happen when we run this program, we can head
over into the assemble to start to see the machine code in action. Note that when
we decompile the binary, we can look for the grader and the main function on the
left scroll bar. Lets get to reversing:

```
function grader:
 1G   push    rbp
 2G   mov     rbp, rsp
 3G   mov     [rbp+var_14], edi
 4G   mov     [rbp+var_18], esi
 5G   mov     edx, [rbp+var_14]
 6G   mov     eax, [rbp+var_18]
 7G   add     eax, edx
 8G   mov     [rbp+var_4], eax
 9G   cmp     [rbp+var_4], 31h
10G   jle     short loc_4004F8

12G   mov     eax, 50h
13G   jmp     short loc_4004FD

loc_4004F8:
14G   mov     eax, 46h

loc_4004FD:
15G   pop     rbp
16G   retn
17G   add endp


function main:
 1M   push    rbp
 2M   mov     rbp, rsp
 3M   sub     rsp, 10h
 4M   mov     [rbp+var_8], 1Ah
 5M   mov     [rbp+var_4], 16h
 6M   mov     edx, [rbp+var_4]
 7M   mov     eax, [rbp+var_8]
 8M   mov     esi, edx
 9M   mov     edi, eax
10M   call    grader
11M   mov     [rbp+var_9], al
12M   mov     eax, 0
13M   leave
14M   retn
15M   main endp

```

Let us go through one by one what this program does. Throughout this analysis, I
will be referring to the line numbers. These are the `5M` / `13G` things on the left
of each of the instructions.

## Walking through main

### Setting up the stack frame

In the previous lesson we covered the use of the `rbp` and `rsp` registers. Note
that they have made their appearance again at the top of the assembly code again.

```
 1M   push    rbp
 2M   mov     rbp, rsp
 3M   sub     rsp, 10h
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

The stack now looks like this. Note that the value for the address is just an
arbitrary choice. I decided to start the absolute bottom at address 0x00, for
simplicity.

|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|  
|     `-0x14`     |   `rbp - 0x10`    |                   |            `rsp` points here          |
|     `-0x10`     |   `rbp - 0x0C`    |                   |                                       |
|     `-0x0C`     |   `rbp - 0x08`    |                   |                                       |
|     `-0x08`     |   `rbp - 0x04`    |                   |                                       |
|     `-0x04`     |   `rbp`           |  Hidden Local var |            `rbp` points here          |
| Absolute Bottom |   `rbp + 0x04`    |    `prev rbp`     |        Meaningless value to us        |

Note that implicitly there will be a hidden local variable stored at location
`[rbp]`. S

### Initialising the variables:

Next we have these two lines of instructions:

```
 4M   mov     [rbp+var_8], 1Ah
 5M   mov     [rbp+var_4], 16h
```

This is simple, it just tells the computer to move the value `0x1A = 26` into the
memory address at `[rbp+var_8]`. The second instruction moves the value `0x16 = 22`
into the memory address at `[rbp+var_4]`.

Oh, side note: in IDA Pro, the memory addresses is shown with an addition sign, as
in `[rbp` *+* `var_4]`. This is because IDA Pro defined `var_4` to have the value
of `-4`. Essentially, when you see `var_x`, it refers to a value of `-x`.

|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|  
|     `-0x14`     |   `rbp - 0x10`    |                   |            `rsp` points here          |
|     `-0x10`     |   `rbp - 0x0C`    |                   |                                       |
|     `-0x0C`     |   `rbp - 0x08`    |      `0x1A`       |                                       |
|     `-0x08`     |   `rbp - 0x04`    |      `0x16`       |                                       |
|     `-0x04`     |   `rbp`           |  Hidden Local var |            `rbp` points here          |
| Absolute Bottom |   `rbp + 0x04`    |    `prev rbp`     |        Meaningless value to us        |


### Preparing to call the function `grader`

```
 6M   mov     edx, [rbp+var_4]
 7M   mov     eax, [rbp+var_8]
 8M   mov     esi, edx
 9M   mov     edi, eax
```

So now, in order to get the correct values into the parameters for the function
`grader`, we first move the two variables into the `edx` and `eax` registers
respectively. Then, we move the two values into `esi` and `edi` respectively.
Remember that input parameters into functions is input into those two registers.

OK crap, so on top of tracing the contents of the stack, we should also keep track
of the variables in the registers as well. Let's introduce that:

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x1A` | `0x16` | `0x1A` | `0x16` | `-0x04` | `-0x14` | `10M`  |

### `call    grader`

```
10M   call    grader
```

Now the function `grader` gets called. Remember that when we use the `call`
instruction, we need to push the return address (which refers to the instruction we
are currently looking) at into the stack. After that, we perform an unconditional
jump to the instructions of function `grader`.

Note that we push in the value `11M` on to the stack because we have already
processed the instruction at `10M`. Always remember to advance the `pc` after reading
each instruction.

|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|
|     `-0x18`     |                   |       `11M`       |     Return address from `grader`      |
|     `-0x14`     |   `rbp - 0x10`    |                   |            `rsp` points here          |
|     `-0x10`     |   `rbp - 0x0C`    |                   |                                       |
|     `-0x0C`     |   `rbp - 0x08`    |       `0x1A`      |                                       |
|     `-0x08`     |   `rbp - 0x04`    |       `0x16`      |                                       |
|     `-0x04`     |   `rbp`           |  Hidden Local var |            `rbp` points here          |
| Absolute Bottom |   `rbp + 0x04`    |     `prev rbp`    |        Meaningless value to us        |


| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x1A` | `0x16` | `0x1A` | `0x16` | `-0x04` | `-0x14` |  `1G`  |

## Function `grader`

### Setting up the frame

```
 1G   push    rbp
 2G   mov     rbp, rsp
```

Once again, we push the current value of `rbp` into the stack, and then move
the value of the old `rsp` into the `rbp`.

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x1A` | `0x16` | `0x1A` | `0x16` | `-0x20` | `-0x20` |  `3G`  |

```
 3G   mov     [rbp+var_14], edi
 4G   mov     [rbp+var_18], esi
```

Now the value of `edi` will be pushed into `[rbp+var_14]`, and the value of `esi`
will be pushed into `[rbp+var_18]`. Note that these two variables are the input
parameters into the function `grader`. This step is to save the value of the input
parameters, so that future calls can get to them. The stack looks like this:


|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|
|     `-0x38`     |    `rbp - 0x18`   |       `0x16`      |                                       |
|     `-0x34`     |    `rbp - 0x14`   |       `0x1A`      |                                       |
|     `-0x20`     |    `rbp - 0x00`   |  Hidden Local var |      `rbp` and `rsp` points here      |
|     `-0x1C`     |                   |      `-0x04`      |       Previous `rbp` for `main`       |
|     `-0x18`     |                   |       `10M`       |     Return address from `grader`      |
|     `-0x14`     |                   |                   |      Top of the frame for `main`      |
|     `-0x10`     |                   |                   |                                       |
|     `-0x0C`     |                   |       `0x1A`      |                                       |
|     `-0x08`     |                   |       `0x16`      |                                       |
|     `-0x04`     |                   |  Hidden Local var |                                       |
| Absolute Bottom |                   |     `prev rbp`    |        Meaningless value to us        |


### Adding of the variables

```
 5G   mov     edx, [rbp+var_14]
 6G   mov     eax, [rbp+var_18]
 7G   add     eax, edx
```

Now, the value at `[rbp+var_14]` gets copied to the `edx` register, and the value at
`[rbp+var_18]` gets copied to the `eax` register. It just seemed like we went a
roundabout way of assigning the value of `testscore` to `edx` and `examscore` to
`eax`. It makes sense though, because each input parameter into the function has a
name. Hence, they should get their own space in the stack!

Anyway, the two values in the registers gets added together. This is what our
register frame looks like now:

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x30` | `0x1A` | `0x1A` | `0x16` | `-0x20` | `-0x20` |  `8G`  |

### Comparing of values

```
 8G   mov     [rbp+var_4], eax
 9G   cmp     [rbp+var_4], 31h
10G   jle     short loc_4004F8
```

We move the value of `eax` into the the stack frame at `[rbp+var_4]`. Directly
after that, we do a `cmp` instruction. For the people who read my short digression,
you would know that subtracts the second parameter from the first parameter. Now,
we set the flags based on the results of the operation:

| Flags | Overflow | Zero | Sign |
|-------|----------|------|------|
| Value |    0     |   0  |   1  |

We execute a `jle` instruction. If you remember, if a `jle` instruction is called,
we will take the jump if the zero flag has value `1` or if the value of the overflow
flag is not the same as the sign flag. The latter is true, and hence we take the
jump. Update the stacks and registers accordingly:


|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|
|     `-0x38`     |    `rbp - 0x18`   |       `0x16`      |                                       |
|     `-0x34`     |    `rbp - 0x14`   |       `0x1A`      |                                       |
|     `-0x24`     |    `rbp - 0x04`   |       `0x30`      |                                       |
|     `-0x20`     |    `rbp - 0x00`   |  Hidden Local var |      `rbp` and `rsp` points here      |
|     `-0x1C`     |                   |      `-0x04`      |       Previous `rbp` for `main`       |
|     `-0x18`     |                   |       `11M`       |     Return address from `grader`      |
|     `-0x14`     |                   |                   |      Top of the frame for `main`      |
|     `-0x10`     |                   |                   |                                       |
|     `-0x0C`     |                   |       `0x1A`      |                                       |
|     `-0x08`     |                   |       `0x16`      |                                       |
|     `-0x04`     |                   |  Hidden Local var |                                       |
| Absolute Bottom |                   |     `prev rbp`    |        Meaningless value to us        |

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x30` | `0x1A` | `0x1A` | `0x16` | `-0x20` | `-0x20` |  `14G` |

### Collapsing the stack frame

```
loc_4004F8:
14G   mov     eax, 46h
```

Now in the instruction in line `14G`, we move the value `0x46` into the `eax`
register. Remember that since we will be returning from the function soon, this
means that the value `0x46` is a return value.

Suppose we are looking at this assembly program, we would not know what the return
type of the function is. There is no indication whether this value is just a number
or a char. We will need to deduce that fact from after we return. Note that we can
tell based on the number of bits the variable gets assigned to.

```
15G   pop     rbp
```

Now, we call the `pop` instruction. It is simply the reverse to the `push`
instruction: first add `0x04` from `rsp` register. Then copy the value of the
memory address into the `rbp` register. In our case, after adding `0x04` from the
`rsp` register, we end up with `-0x1C`. Then we go to the stack, find the data that
lives at the address `-0x1C` and copy his value into `rbp`.

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x46` | `0x1A` | `0x1A` | `0x16` | `-0x04` | `-0x1C` |  `14G` |

```
16G   retn
17G   add endp
```

Last, we call the `ret` instruction. This instruction pops from the stack one more
time. But this time, the value goes into the `pc` register. So we add `0x04` to
`rsp` to get `-0x18`. Then, we copy the value of that memory address (which in this
case is `10M`) into `pc`.

Note that the previous stuff from the memory doesn't get cleared. The program will
automatically overwrite it as new stack frames are being created and destroyed.

|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|
|     `-0x38`     |    `rbp - 0x18`   |       `0x16`      |           Left over Garbage           |
|     `-0x34`     |    `rbp - 0x14`   |       `0x1A`      |           Left over Garbage           |
|     `-0x24`     |    `rbp - 0x04`   |       `0x30`      |           Left over Garbage           |
|     `-0x20`     |    `rbp - 0x00`   |  Hidden Local var |           Left over Garbage           |
|     `-0x1C`     |                   |      `-0x04`      |           Left over Garbage           |
|     `-0x18`     |                   |       `11M`       |           Left over Garbage           |
|     `-0x14`     |                   |                   |           `rsp` points here           |
|     `-0x10`     |                   |                   |                                       |
|     `-0x0C`     |                   |       `0x1A`      |                                       |
|     `-0x08`     |                   |       `0x16`      |                                       |
|     `-0x04`     |                   |  Hidden Local var |           `rbp` points here           |
| Absolute Bottom |                   |     `prev rbp`    |        Meaningless value to us        |

| Registers | `eax`  | `edx`  | `edi`  | `edi`  |  `rbp`  |  `rsp`  |  `pc`  |
|-----------|--------|--------|--------|--------|---------|---------|--------|
| DataValue | `0x46` | `0x1A` | `0x1A` | `0x16` | `-0x04` | `-0x1C` |  `11M` |

## The final return to main

```
11M   mov     [rbp+var_9], al
```

Now, notice that we are moving the data in the `al` register. Referring back to the
[ripped off register from wikipedia][registers], we realise that this value is the
smallest 8 bits of the `eax` register. The size of the `al` register is therefore
only 1 byte. There is only one data type we learnt that is 1 byte large: the `char`.
So we can now be sure that the return value of function `grader` is of type `char`.

|     Address     |   Address Value   |  Stack Contents   |                Remarks                |
|-----------------|-------------------|-------------------|---------------------------------------|
|     `-0x38`     |                   |       `0x16`      |           Left over Garbage           |
|     `-0x34`     |                   |       `0x1A`      |           Left over Garbage           |
|     `-0x24`     |                   |       `0x30`      |           Left over Garbage           |
|     `-0x20`     |                   |  Hidden Local var |           Left over Garbage           |
|     `-0x1C`     |                   |      `-0x04`      |           Left over Garbage           |
|     `-0x18`     |                   |       `11M`       |           Left over Garbage           |
|     `-0x14`     |    `rbp - 0x14`   |                   |           `rsp` points here           |
|     `-0x0D`     |    `rbp - 0x09`   |       `0x46`      |             `char` value              |
|     `-0x0C`     |    `rbp - 0x08`   |       `0x1A`      |                                       |
|     `-0x08`     |    `rbp - 0x04`   |       `0x16`      |                                       |
|     `-0x04`     |    `rbp - 0x00`   |  Hidden Local var |           `rbp` points here           |
| Absolute Bottom |                   |     `prev rbp`    |        Meaningless value to us        |

```
12M   mov     eax, 0
13M   leave
```

And lastly, as we prepare to finish the main function, we move the value `0` into
the return value register `eax`, and `leave` the `main` function.

Which leaves one question... What exactly did we get from the `grader` function?
Converting the `char`: `0x46 = 'F'`.

Oh damn, we failed.

[registers]: ../1_reading_x86/images/registers.png
