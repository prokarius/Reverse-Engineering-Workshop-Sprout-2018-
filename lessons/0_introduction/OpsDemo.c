#include<stdio.h>

int main(){
    int x, y;

    printf ("Input your desired value of x:");
    scanf ("%d", &x);
    printf ("Input your desired value of y:");
    scanf ("%d", &y);

    // Addition
    printf ("The result of %d + %d is %d.\n", x, y, x+y);

    // Subtraction
    printf ("The result of %d - %d is %d.\n", x, y, x-y);

    // Bitwise AND
    printf ("The result of %d & %d is %d.\n", x, y, x&y);

    // Bitwise OR
    printf ("The result of %d | %d is %d.\n", x, y, x|y);

    // Bitwise XOR
    printf ("The result of %d ^ %d is %d.\n", x, y, x^y);

    // Left shift (Multiplying x by 2**y)
    printf ("The result of %d << %d is %d.\n", x, y, x<<y);

    // Right shift (Dividing x by 2**y)
    printf ("The result of %d >> %d is %d.\n", x, y, x>>y);

    // Demonstration of boolean values
    printf ("The result of %d == %d is %d.\n", x, y, x==y);

    return 0;
}
