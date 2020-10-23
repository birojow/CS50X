#include <stdio.h>
#include <cs50.h>
#include <math.h>

const long VISA_13_MIN = 4000000000000;         // interval
const long VISA_13_MAX = 4999999999999;         // of possible
const long VISA_16_MIN = 4000000000000000;      // numbers
const long VISA_16_MAX = 4999999999999999;      // for each
const long AMEX_34_MIN = 340000000000000;       // credit
const long AMEX_34_MAX = 349999999999999;       // card
const long AMEX_37_MIN = 370000000000000;       // company
const long AMEX_37_MAX = 379999999999999;
const long MC_MIN = 5100000000000000;
const long MC_MAX = 5599999999999999;

long get_card_number(void);
bool check_input_validity(long number);
void check_card_operator(long number);
bool verify_checksum(long number);

int main(void) {
    long card_number = get_card_number();
    check_card_operator(card_number);
}

long get_card_number(void) {
    bool input_is_valid = false;
    long card_number = 0;
    while (!input_is_valid) {
        card_number = get_long("Number: ");

        // ignores numbers equal 0 or less
        input_is_valid = check_input_validity(card_number);
    }
    return card_number;
}

bool check_input_validity(long number) {
    if (number < 0) {
        return false;
    }
    else {
        return true;
    }
}

void check_card_operator(long number) {
    if ((number >= VISA_13_MIN && number <= VISA_13_MAX) || (number >= VISA_16_MIN && number <= VISA_16_MAX)) {
        if (verify_checksum(number)) {
            // if the number complies to VISA standard AND the checksum is ok, is a VISA
            printf("VISA\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else if ((number >= AMEX_34_MIN && number <= AMEX_34_MAX) || (number >= AMEX_37_MIN && number <= AMEX_37_MAX)) {
        if (verify_checksum(number)) {
            printf("AMEX\n");       // if the number complies to AMEX standard AND the checksum is ok, is a AMEX
        }
        else {
            printf("INVALID\n");
        }
    }
    else if (number >= MC_MIN && number <= MC_MAX) {
        if (verify_checksum(number)) {
            printf("MASTERCARD\n");     // if the number complies to MC standard AND the checksum is ok, is a MC
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");        // any number that nof fit in AMEX, VISA or MC is invalid
    }
}

bool verify_checksum(long number) {
    // i didn't like the approach I used in this function. I wanted to cast the card
    // number as a string in order to manipulate it easily, but didn't wanted to use
    // functions not presented at the lectures. Well, I used floor(), but at least it
    // is in the already known and used math library. In resume, this function uses
    // the floor(number / 10) * 10 to turn XXXXX into XXXX0, then subtracts XXXX0 from
    // XXXXX to get the last number.

    bool digit_in_even_position = false;
    int even_digits_sum = 0;
    int odd_digits_sum = 0;
    int number_last_digit = 0;
    while (number > 0) {
        if (number > 9) {
            // isolates the last number on another variable
            number_last_digit = number - floor(number / 10) * 10;

            // excludes the last number from the original value
            number = floor(number / 10);
        }
        else {
            number_last_digit = number;
            number = 0;
        }
        if (digit_in_even_position) {
            if (number_last_digit * 2 >= 10) {
                // this is necessary in order to sum the digits of products greater than 9
                even_digits_sum += number_last_digit * 2 - 10 + 1 ;
            }
            else {
                even_digits_sum += number_last_digit * 2;
            }
        }
        else {
            odd_digits_sum += number_last_digit;
        }

        // this flag keeps track of the position of the number
        digit_in_even_position = !digit_in_even_position;
    }

    if ((even_digits_sum + odd_digits_sum) % 10 == 0) {
        return true;
    }
    else {
        return false;
    }
}