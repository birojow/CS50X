#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool check_arguments(int argc);
bool check_key(string key);
bool check_repetitions(string key);
void encrypt_and_print(string plaintext, string key);

int main(int argc, string argv[])
{
    bool argument_is_invalid = check_arguments(argc);
    if (argument_is_invalid) {
        return 1;
    }
    string key = argv[1];
    bool key_is_invalid = check_key(key);
    if (key_is_invalid) {
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    encrypt_and_print(plaintext, key);
    return 0;
}

bool check_arguments(int arg_count) {
    if (arg_count != 2) {
        printf("Usage: ./substitution key");
        return true;
    }
    return false;
}

bool check_key(string key) {
    if (strlen(key) != 26) {
        printf("Key must contain 26 characters");
        return true;
    }
    for (int i = 0; i < strlen(key); i++) {
        if (!isalpha(key[i])) {
            printf("Key must contain letters only");
            return true;
        }
    }
    bool repetition = check_repetitions(key);
    if (repetition) {
        return true;
    }
    return false;
}

bool check_repetitions(string key) {
    for (int i = 0; i < strlen(key) - 2; i++) {
        for (int j = i + 1; j < strlen(key); j++) {
            if (key[i] == key[j]) {
                return true;
            }
        }
    }
    return false;
}

void encrypt_and_print(string plaintext, string key) {
    printf("ciphertext: ");
    for (int i = 0; i < strlen(plaintext); i++) {
        if (islower(plaintext[i])) {
            char character = tolower(key[plaintext[i] - 'a']);
            printf("%c", character);
        }
        else if (isupper(plaintext[i])) {
            char character = toupper(key[plaintext[i] - 'A']);
            printf("%c", character);
        }
        else {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}