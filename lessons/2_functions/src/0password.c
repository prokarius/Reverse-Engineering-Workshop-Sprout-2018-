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

    password = ((password - 14) << 2) + 9;

    if (password != 173){
        printf ("WRONG PASSWORD!\n");
        return 0;
    }

    printf ("THANK YOU ADMIN!\n");
    printf ("BUT OUR PRINCESS IS IN ANOTHER CASTLE!\n");

    return 0;
}
