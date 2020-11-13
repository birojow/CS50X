from cs50 import get_int

invalid_input = True
while invalid_input:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        invalid_input = False

for step in range(height):
    hashtags = '#' * (step + 1)
    print(hashtags.rjust(height, ' '))