#include <stdio.h>
#include <cs50.h>

int main(void) {
    // receive user input and stores at variable 'name'
    string name = get_string("What is your name?\n");
    // greets the user by name
    printf("hello, %s\n", name);
}