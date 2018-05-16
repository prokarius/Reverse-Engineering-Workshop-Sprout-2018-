#include<stdio.h>
#include<string.h>

int main(){
    char username[2000];
    char user[2000] = "admin";
    int password;

    printf ("Enter your username: ");
    scanf ("%s", username);

    printf ("What is your password: ");
    scanf ("%d", &password);

    if (strcmp(username, user) != 0){
        printf ("YOU ARE NOT THE ADMIN!\n");
        return 0;
    }

    password -= 14;
    password <<= 2;
    password += 9;

    if (password != 173){
        printf ("WRONG PASSWORD!\n");
        return 0;
    }

    printf ("Welcome, admin! Here is the super secret sauce!\n");
    printf ("sprout{crappy_login_system}");

    return 0;
}
