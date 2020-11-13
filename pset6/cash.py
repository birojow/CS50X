from cs50 import get_float

invalid_input = True

while invalid_input:
    change_owed = get_float('Change owed: ')
    if change_owed > 0:
        invalid_input = False

coins = 0
change_owed = int(change_owed * 100)
while change_owed >= 25:
    change_owed -= 25
    coins += 1
while change_owed >= 10:
    change_owed -= 10
    coins += 1
while change_owed >= 5:
    change_owed -= 5
    coins += 1
while change_owed >= 1:
    change_owed -= 1
    coins += 1

print(coins)