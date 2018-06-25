# Loops and Recursion

Now that we have learnt to call function, we just have one last piece of the puzzle
to piece together: How does a computer do things repeatedly? This is about looping 
and recursion.

Now, suppose we have a program that does a simple function, such as grading a piece
of homework. We want it to be able to repeatedly call it for every single piece of
homework we have, so that everything gets graded. From what we have learnt though,
we are still unable to create such a construct. It's time to learn it:

## Looping in C

The C programming language allows us two (basic) ways to create loops within our
program: `for` loops and `while` loops. In this workshop, I cover the syntax of a
while loop in C. This is because after we disassemble the x86 code it is easier to
see the resulting code to be in the format of a `while` loop. The syntax of loops in
assembly code is very similar to a `while` loop's. 

Although I give much emphasis on to the `while` loop, we should still learn the syntax
of both types of loops. Partially because the `for` loop may sometimes give a better
idea of what a program is doing. Having mentioned this, lets dive in:


## Syntax of a while loop

In C, the syntax of writing a while loop is pretty simple. Let's see what a simple
while loop looks like:

```
while (condition){
    loop body
}
```

Yes, it's that simple. `condition` is the result of a computation that returns a
`true` or `false` value, and `loop body` is the piece of code you want done for the
time the `condition` remains true.

"Wait, didn't you mention that C does not have any constructs for `boolean`s in the
first lesson, when you mentioned about data types?". Yes. Good that you caught that.
Now suppose I make a comparison as `condition`, that should give a `true` or `false`
result. Except, in C, we represent `true` and `false` as `1` and `0` respectively.
In this case,

What this also means is we can put a numerical value into `condition` and it will
be evaluated to either `true` or `false`. That makes sense if the value is `1` or
`0`, but what if the value is something else? Turns out in C, values that are not
`0` are all considered as `true`. As long as there is a bit that is on within the
data, it would be considered as `true`.


## Example of a loopy program

Suppose you want to write a program to help you calculate the nth fibonacci number. 
Let's see how this would be set up in first the C language, and then try to analyse
the decompiled version.

First we need to get the user to key in which fibonacci number they would like. We
initialise an `int` variable and call the `scanf` function to input the user's input
into the `int`:

```
int n;
scanf("%d", &n);
```

Remember, the 0th and 1st fibonacci numbers are `0` and `1` respectively. We need
to initialise those two numbers first. Perhaps we can start by setting two variables
to `0` and `1` respectively. Let's call the two variables `x` and `y`. How do we get
the next fibonacci number? You need to add the values of `x` and `y`. Lets put set
the result of `x` + `y`  to `z`. Then we can print it out:

```
int x = 0;
int y = 1;
int z = x + y;
printf ("%d\n" y);
```

Now that only gives us the value of the 2nd fibonacci number. How should we do it if
we wanted to calculate more fibonacci numbers? Suppose we want `z` to contain the
kth fibonacci number. Then we note that `x` should contain the value of the (k-2)th
fibonacci number, and similarly, `y` should contain the value of the (k-1)th
fibonacci number. Hence within the loop, we need set up the variables as such.

Now, say we somehow managed to complete one run of the loop. The varible `x`, `y`
and `z` are set to their respective numbers. Now, how do we calculate the next
fibonacci number? Well, the value in `y` simply needs to move over to the value of
`x`, the value of `z` simply needs to move over to the value of `y`. Then we can add
these two back together to form the next value of `z`. Lets write that code:

```
x = y;
y = z;
z = x + y;
```

Ok, now we need to think about the logic. Suppose we run the above code 1 time.
Then the value of `y` will be the 1st fibonacci number. If we ran the code 2 times,
then the value of `y` will be the 2nd fibonacci number. Hence we need to run the
loop `n` times! Let's keep a counter that starts at `0` and counts up.

```
int counter = 0;
while (counter < n){
    code;
    counter++; // counter = counter + 1
}
```

So once again, this will loop until counter is equal to `n-1`, then it will stop
looping. The total number of times it loops: `n-1` times.

We can also do this another way: counting down from `n-2`, and when the value
reaches `0`, we can just end the loop:

```
n -= 1;
while (n){
    code;
    n--;
}
```

Incidentally, what the code `x++` does is to increase the value of `x` by 1.
Similarly, the code `y--` decreases the value of `y` by 1.

Now before the loop runs, what should the original values of `x`, `y` and `z` be?
Notice that the value of `y` and `z` needs to be the two consecutive fibonacci
numbers. The initial value of `x` doesn't matter. What are the two consecutive
numbers in question? `0` and `1`!, lets initialise them:

```
int x;
int y = 0;
int z = 1;
```

Alright, so now that we have everything together, lets put all the code together!
Add a couple of helpful prompts and change out return statements into output that
people can understand what we are doing. The final file can be found by following
[this link][fibonacci].

```
#include<stdio.h>

int main(){
    printf ("Which fibonacci number do you want to find out?\n");
    int n;
    scanf ("%d", &n);

    int x;
    int y = 0;
    int z = 1;
    int counter = 0;
    while (counter < n){
        x = y;
        y = z;
        z = x + y;
        counter++;
    }

    printf ("The value of the %dth fibonacci number is %d.\n", n, y);
}
```

Now, to see how to disassemble this program, go to this [extra lesson][fibodis].

[fibonacci]: ./src/1_fibonacci.c
[fibodis]: ./fibonacci.md

### Integer Overflow

Right, our program seems to be working pretty well for small numbers, but what
happens when we try to ask the program for the 47th fibonacci number?

```
Which fibonacci number do you want to find out?
47
The value of the 47th fibonacci number is -1323752223.
```

Oh boy. Suddenly the 47th fibonacci number is a negative number? That can't be
right. As we have mentioned in the first lesson, this is a working example of
*integer overflow*.

Since we are working with `int`s, the computer only has 32 bits of memory per
variable, one of which is reserved for the sign of the number, leaving 31 bits for
the actual numerical representation itself. What is the largest possible total that
this particular variable can store? That's `2**31 = 2147483648`. Note that the real
value of the 47th fibonacci number is `2971215073`, which is just slightly above the
maximum number that can be stored in an `int`. Hence the binary representation of
the number overflows and has entered into the value of the sign bit.


## RIR : Recusion Is RIR

It was once said that to understand recursion, you first need to understand
recursion. This feels more like circular dependencies, but I digress. 

Recursion is when we call a function that we have defined within that same
particular function. Lets see this in action using the idea behind the factorial
function:

The factorial of a given integer n is given by the product of all integers lower
than it, until reaching 1. For example `5! = 5 * 4 * 3 * 2 * 1`. But notice that
`4! = 4 * 3 * 2 * 1`. That means we can express `5!` in terms of `4!` as such:
`5! = 5 * 4!`. And notice that `3! = 3 * 2 * 1`, so we can express `4!` in terms of
`3!` as such: `4! = 4 * 3!`.

What this means is that we can recursively define the factorial function:
`n! = n * (n-1)!`. This is the meaning of recursion: defining a function in terms of
itself. In fact, what we have just shown above is called the *recursive step*.

Now this makes sense, but when do we stop? If we can say `1! = 1 * 0!` and then
`0! = 0 * -1!`, and `-1! = -1 * -2!`. This is starting to make no sense. What we
need to introduce is called the *base case*. It tells us when we are supposed to
stop the recursion. In the factorial function's case, we have defined `0! = 1` and
thus, this shall be our base case. We can then define the factorial function as:

```
0! = 1
n! = n * (n-1)!
```

Analysis of an example piece of recursive code will be done as an
[extra lesson][recursive].

[recursive]: ./recursion.md

## I am the master of the loopy boiis!!

```
Good. 
Lets play a game then. 
See this bomb? It has 15 minutes until it goes kaboom.
Only if you enter a correct UserID and passcode, the bomb will be defused.
You have 15 minutes.
```

Oh no what's this suspecious looking package on the ground?! Ah! It works on x86
assembly?! Hey we just might be able to defuse this! Use the knowledge that you have
learnt in order to defuse the bomb (and potentially get some turtl as well).

Remember this test goes through everything we have learnt so far. 

Good luck!

# Where do we go from here?

Congrats on making it to the end of this short workshop. For those who already know
what the ideas and paradigms of this workshop, I hope this was a good refresher in
terms of the technicalities of certain aspects of reverse engineering and the
architecture of the x86 operating system.

For those students who have a tad bit of programming experience, I hope this
workshop has showed you programming as a layer deeper than what you already know, to
give a deeper understanding of how the computer works as well. While the workshop
was pitched at a level where even beginner students can understand, I still try to
include in certain points which are more advanced topics.

I know I have glossed over much of the technically advanced stuff, but please,
should you find yourself wondering about something, go and explore it yourself.
Before creating this course I have always thought that when we enter a new stack
frame of a function, the data at memory `[rbp]` would not be used, as it was the
way for the computer to keep track where the frame started and ended. Now I know
this is not true, because there exists some local hidden variable in there.

And lastly for those people who previously do not have any computer science
experience, I hope you left this workshop understanding more about the machines that
surrounds you. I hope that you understand the amount of thought that was put into
the design and architecture of a computer, and that while we take it for granted
most of the time in our everyday lives, somewhere inside that thinking rock of
yours, a couple hundreds of little instructions are being executed.

I hope that you start to realise that our human life is kinda like a program as
well. Apart from the random and spontaneous ideas we sometimes do have, the rest of
the time we spend here are really just a bunch of if-else statements. And complex
tasks like going from the canteen back to class is really a very long piece of
instruction, that is broken up into smaller parts, each one being done with less
conscious thought on it. In a sence we really are like computers, following
insturctions (within our brain) as well. I challenge you to start viewing the
world in a more methodological manner, and see if you could see the `"code"`
that makes up our thinking mind.

While I have tried to dumb down the content, there will always be gaps and holes
that I have missed or failed to fix, do let me know either by just letting me know.

Should the turn out for this year's sprout be good, I might decide to continue this
on to next year's sprout. Potential topics include:

1. Arrays and Structures (Has storing things together always been this difficult?)
2. Floating Point Registers (We're been working with integers, no decimal point!)
3. Basic Exploitation (Where we make the computer do things it's not supposed to.)
4. Reading the Java Bytecode.

With that, thank you for staying with me to the end of this course.

```
pop    rbp
retn 
```
