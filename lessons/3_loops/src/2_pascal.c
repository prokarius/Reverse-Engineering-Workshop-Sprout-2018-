#include<stdio.h>

int pascal (int n, int r){




}

int main(){
    int n, r;
    printf("Simple Program to find out nCr.\n");
    printf("Enter n: ");
    scanf("%d", &n);
    printf("Enter r: ");
    scanf("%d", &r);

    int output = pascal(n, r);
    printf ("%d choose %d is %d.\n", n, r, output);
}
