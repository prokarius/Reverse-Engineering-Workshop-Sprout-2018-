#include<stdio.h>

int rcs(unsigned int x, unsigned int y){
    unsigned int out = 0;
    while (y != 0){
        if (y & 1){
            out += x;
        }
        x <<= 1;
        y >>= 1;
    }
    return out;
}

int main(){
    int x, y;
    printf ("Enter UserID: ");
    scanf ("%d", &x);

    printf ("Enter Passcode: ");
    scanf ("%d", &y);

    if (x < 10000 || y < 10000){
        printf ("Numbers are too small.\n");
        return 0;
    }

    int out = rcs(x, y);

    if (out == 110447259){
        printf ("You win!\n");
        printf ("flag{the_password_is_ReverseEngineering2018}\n");
    }

    else {
        printf ("Wrong Answer\n");
    }

    return 0;
}
