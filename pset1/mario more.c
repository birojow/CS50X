#include <stdio.h>
#include <cs50.h>

int get_size();
bool check_size(int size);
void build_pyramid(int size);
void print_spaces(int spaces);
void print_hashtags(int hashtags);

int main(void) {
    int size = get_size();
    build_pyramid(size);
}

int get_size() {
    int size = 0;
    bool size_is_valid = false;
    while (!size_is_valid) {
        size = get_int("Height: ");
        size_is_valid = check_size(size);
    }
    return size;
}

bool check_size(int size) {
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
    while (hashtags <= size) {
        spaces = size - hashtags;
        print_spaces(spaces);
        print_hashtags(hashtags);
        printf("  ");                       // just added these 3 lines
        print_hashtags(hashtags);           // in order to go from mario less
        printf("\n");                       // to mario more
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
}