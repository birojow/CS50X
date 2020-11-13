from cs50 import get_string
def main():
    number = get_string("Number: ")
    size = len(number)

    if size < 13 or size > 16:
        print("INVALID\n")
    else:
        if (size == 13 or size == 16) and number[0] == '4':
            is_valid = check_validity(number)
            if is_valid:
                print("VISA\n")
            else:
                print("INVALID\n")
        elif size == 15 and number[0] == '3' and (number[1] == '4' or number[1] == '7'):
            is_valid = check_validity(number)
            if is_valid:
                print("AMEX\n")
            else:
                print("INVALID\n")
        elif size == 16 and number[0] == '5' and (int(number[1]) > 0 and int(number[1]) < 6):
            is_valid = check_validity(number)
            if is_valid:
                print("MASTERCARD\n")
            else:
                print("INVALID\n")
        else:
            print("INVALID\n")

def check_validity(number):
    odd_digits_sum = 0
    even_digits_sum = 0
    digit = len(number) - 1
    while digit > -1:
        if (len(number) - digit) % 2 != 0:
            odd_digits_sum += int(number[digit])
        else:
            digit_double = int(number[digit]) * 2
            if digit_double > 9:
                digit_double = str(digit_double)
                for i in digit_double:
                    even_digits_sum += int(i)
            else:
                even_digits_sum += digit_double
        digit -= 1
    total_sum = even_digits_sum + odd_digits_sum
    if total_sum % 10 == 0:
        return True
    return False

main()