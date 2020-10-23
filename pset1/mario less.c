#include <stdio.h>
#include <cs50.h>

int get_size();
bool check_size(int size);
void build_pyramid(int size);
void print_spaces(int spaces);
void print_hashtags(int hashtags);

int main() {
    int size = get_size();
    build_pyramid(size);
}

int get_size() {
    int size = 0;
    bool size_is_valid = false;
    // keep asking for a value until a valid one is given
    while (!size_is_valid) {                                                   
        size = get_int("Height: ");
        size_is_valid = check_size(size);
    }                                                  
    return size;
}

bool check_size(int size) {
    // these constants allow the configuration
    // of the size of the pyramid
    const int MINIMUM_SIZE = 1;                         
    const int MAXIMUM_SIZE = 8;                         
    if (size < MINIMUM_SIZE || size > MAXIMUM_SIZE) {
        return false;
    }
    else {
        return true;
    }
}

void build_pyramid(int size) {
    int spaces;
    int hashtags = 1;

    // this loop prints spaces and hashtags using separate
    // functions. the number of characters to be printed is
    // passed as arguments. after each line printed,
    // the number of hashtags for the next line is updated.
    // the number of spaces is calculated relative to hashtags
    while (hashtags <= size) {
        spaces = size - hashtags;
        print_spaces(spaces);
        print_hashtags(hashtags);
        hashtags++;
    }
}

void print_spaces(int spaces) {
    for (; spaces > 0; spaces--) {
        printf(" ");
    }
}

void print_hashtags(int hashtags) {
    for (; hashtags > 0; hashtags--) {
        printf("#");
    }
    printf("\n");
}