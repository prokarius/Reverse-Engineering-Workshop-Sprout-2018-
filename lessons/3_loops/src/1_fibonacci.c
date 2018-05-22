#include<stdio.h>

int main(){
    printf ("Which fibonacci number do you want to find out?\n");
    int n;
    scanf ("%d", &n);

    if (n == 0){
        printf ("The value of the 0th fibonacci number is 0.\n");
    }

    int x;
    int y = 0;
    int z = 1;
    int counter = 0;
    while (counter < n-1){
        x = y;
        y = z;
        z = x + y;
    }

    printf ("The value of the %dth fibonacci number is %d.\n", n, z);
}
