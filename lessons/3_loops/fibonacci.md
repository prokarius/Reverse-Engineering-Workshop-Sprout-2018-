# Extra Lesson: Disassembling the fibonacci program

Right, now that we have gotten this out of the way, we can disassemble
[the program][fibo]:

```
push    rbp
mov     rbp, rsp
sub     rsp, 20h
mov     rax, fs:28h
mov     [rbp+var_8], rax
xor     eax, eax
```

More stuff setting up the stack.

```
mov     edi, offset s   ; "Which fibonacci number do you want to f"...
call    _puts
```

The program calls the `puts` function with the string `"Which fibonacci..."`.

```
lea     rax, [rbp+var_1C]
mov     rsi, rax
mov     edi, offset aD  ; "%d"
mov     eax, 0
call    ___isoc99_scanf
```

Then we move


mov     eax, [rbp+var_1C]
test    eax, eax
jnz     short loc_40068E
```



[fibo]: ./src/fibonacci
