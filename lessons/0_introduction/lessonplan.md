# A small Introduction to C Programming

As students to this workshop might not be programmers, this short lesson would
serve as a primer to understanding how x86 assembly works.


## Data Types

Before we talk about C programs, we need to mention the specific data types that are
present in computing languages. In a computer, data exists as bits (Binary digITS) 
of 1s and 0s. These are grouped into chunks of 8 bits called a byte. This should be
a familiar measure of size for the non-technical person.

During this workshop, you will see various data types being used. We will go through
two of the most basic data types that will be used.

An `int` is the most basic numerical unit. It represents an **int**eger that has 32
bits. The value of the integer is given by the binary value of the bits that it is
made up of. E.g. 00001010 = value 10

A `char` is an ASCII **char**acter. Remember that strings of text are really just
a bunch of characters put together. Note however, that there is a special character
to demarcate the end of the string -- the `NULL` byte. It is a character whose bits
are all 0s.

However, in C, there is no such thing as a definite `true`/`false` value (these values
are called `boolean` values btw). In C, only the number 0 returns `false`, every
other number would return `true`. Keep this in mind as we go through the reversing
C programs segment later.

Note that there are other types such as `long`, `float`, `double`, etc. These will
not be covered in this workshop. Arrays and the like would also not be covered in
this session. The assembly instructions regarding these other data types are also
more involved. Perhaps these would be covered in future workshops (Sprout 2019?).


## Structure of a C program

In a C program, there would be functions, statements and expressions.

Statements are code that do not return a value. An example of a statement would be
an if-else statement. These are _conditional_ statements that allow for the computer
to make decision based on fulfilled or unfulfilled conditions.

Another example would be a variable declaration statement. These statements can be
identified beginning with a type and followed by a string of characters that
represent the name of the variable. This can be thought of in the same was as
algebra. From that point on, you would be able to use the variable with its name.

Here is an example: 

```
int x = 5; // Variable Declaration Statement

if (x == 5){ // If-else statement
    printf ("The value of x is 5.\n");
}
else{
    printf ("The value of x is not 5.\n");
}
```


Expressions are lines of code that return a specific value. For example, `4 + 2`
is an expression, because it returns the value `6`. Note that things which return
the value of `void` is also considered an expression. As a rule of thumb, as long as
there is a return value, it would be considered an expression. Similarly, if a
function was declared with `int check()`, the piece of code `check()` would be an
expression. More on functions now.

Functions are chunks of code that have been grouped together. The reasons
for doing so are many: First it could be because the program needs to call the same
piece of code multiple times, hence it makes sense to put every thing away. It could
also be used for code grouping.

Functions are identified by a function declaration. This tells the compiler about a
function's name, return type, and parameters. A function definition provides the 
code the program is supposed to run when the function is called.

Calling a function is simple: you take the function's name, and append parenthesis
(`()`) to it. Within the parenthesis, you will input any parameters that the
function takes, in the correct type. Suppose a function declaration states
`int function1(int x, int y, char z){}`. Then a proper call to the function would be
(for example) `function1(1, 2, 'a');`.

Let's look at another example:

```
int add(int x, int y){
    return x + y;
}

int a = 3;
int b = 6;
int z = add(a, b);

```

In the above example, the name of the function is add, and the return type is `int`.
It takes in two `int`s, and within the function, the names of the passed in values
are called x and y.

When the function is called, the values of 3 and 6 are assigned to the variables
a and b respectively. The function `add` is then called with the value in a and b,
i.e. we will have `add(3, 6)`. 
In the function `x` and `y` will therefore values `3` and `6` respectively. Then the
function would then return x + y = 3 + 6 = 9. This is then assigned to the value z.

The most important function in a C program would be the main function. It tells
the computer that the lines of code within it starts the program. The program would
run until the end of the main function, after which the program would end.


## Quick Check:
Within the following piece of code, what

```
int main(){
    int x, y;

    // Reads in two numbers, and stores it in x and y
    scanf("%d %d", &x, &y);
    int z = x + y;

    if (z == 3){
        printf ("flag");
    }

    return 0;
}
```
