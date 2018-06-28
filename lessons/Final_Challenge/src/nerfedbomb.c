#include<stdio.h>

int rcs(unsigned int x, unsigned int y){
    unsigned int out = 0;
    while (y != 0){
        out += x;
        y--;
    }
    return out;
}

int main(){
    int x, y;
    printf ("Enter UserID: ");
    scanf ("%d", &x);

    printf ("Enter Passcode: ");
    scanf ("%d", &y);

    if (x < 10000 || y < 100){
        printf ("Numbers are too small.\n");
        return 0;
    }

    int out = rcs(x, y);
    out = rcs(out, y);

    if (out == 330326373){
        printf ("You win!\n");
        printf ("flag{the_password_is_ReverseEngineering2018}\n");
    }

    else {
        printf ("Wrong Answer\n");
    }

    return 0;
}
