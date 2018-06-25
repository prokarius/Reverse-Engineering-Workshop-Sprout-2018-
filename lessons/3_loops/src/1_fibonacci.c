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
