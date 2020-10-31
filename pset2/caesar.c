#include <cs50.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int check_arguments(int arg_count, string arg_vector[]);
void encrypt_and_print(string plaintext, int key);

int main(int argc, string argv[]) {
    int is_argument_valid = check_arguments(argc, argv);
    if (is_argument_valid == 1) {
        return 1;
    }
    int key = strtoimax(argv[1], 0, 10);        // happy that I found this function
    string plaintext = get_string("plaintext: ");
    encrypt_and_print(plaintext, key);
    return 0;
}

int check_arguments(int arg_count, string arg_vector[]) {
    if (arg_count != 2) {
        printf("Wrong number of arguments. Please insert only one argument.\n");
        return 1;   // I would use the same error message here. But I don't like to be lazy
    }
    for (int i = 0; i < strlen(arg_vector[1]); i++) {
        if (arg_vector[1][i] < '0' || arg_vector[1][i] > '9') {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    return 0;
}

void encrypt_and_print(string plaintext, int key) {
    if (key > 25) {
        key = key % 26;
    }
    printf("ciphertext: ");
    char character;
    for (int i = 0; i < strlen(plaintext); i++) {
        if ((islower(plaintext[i]) && plaintext[i] + key > 'z') || (isupper(plaintext[i]) && plaintext[i] + key > 'Z')) {
            character = plaintext[i] + key - 26;
        }
        else if ((islower(plaintext[i]) && plaintext[i] + key <= 'z') || (isupper(plaintext[i]) && plaintext[i] + key <= 'Z')) {
            character = plaintext[i] + key;
        }
        else {
            character = plaintext[i];
        }
        printf("%c", character);
    }
    printf("\n");
}