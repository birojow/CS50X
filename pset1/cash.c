#include <stdio.h>
#include <math.h>
#include <cs50.h>

float get_change_owed();
bool check_change(float value);
int calculate_coins(int change);

int main(void) {
    float dollars = get_change_owed();

    // converting dollars to cents, and float to int
    int cents = round(dollars * 100);
    int change_coins = calculate_coins(cents);
    printf("%i\n", change_coins);
}

float get_change_owed() {
    float change_owed = 0;
    bool change_is_valid = false;

    // this loop will run until the user enters a valid value
    while (!change_is_valid) {
        change_owed = get_float("Change owed: ");
        change_is_valid = check_change(change_owed);
    }
    return change_owed;
}

bool check_change(float value) {
    // this functions rejects numbers equal 0 or negative
    if (value <= 0) {
        return false;
    }
    else {
        return true;
    }
}

int calculate_coins(int change) {
    int coins = 0;

    // calculate quarters
    while (change >= 25) {
        change -= 25;
        coins++;
    }

    // calculate dimes
    while (change >= 10) {
        change -= 10;
        coins++;
    }

    // calculate nickels
    while (change >= 5) {
        change -= 5;
        coins++;
    }

    // calculate pennies
    while (change >= 1) {
        change--;
        coins++;
    }
    return coins;
}