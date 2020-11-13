import csv
from math import floor
from sys import argv, exit

# check number of arguments
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# store file paths
database_path = argv[1]
sequence_path = argv[2]

# load dna
with open(sequence_path, mode='r') as text_file:
    sequence = text_file.read()

# load database
csv_file = open(database_path, 'r')
database = csv.DictReader(csv_file)

# create list of STRs
str_list = []
for i in database.fieldnames:
    if i != 'name':
        str_list.append(i)

# create list to store matches of each STR
matches= []
for i in str_list:
    matches.append(0)

# look for the max STR repetitions
for i in range(len(str_list)):
    max_repetitions = floor(len(sequence) / len(str_list[i]))
    while max_repetitions > 0:
        if sequence.count(str_list[i] * max_repetitions) != 0:
            matches[i] = str(max_repetitions)
            break
        max_repetitions -= 1

# compare STR repetitions with database
found = False
for row in database:
    for i in range(len(str_list)):
        if row[str_list[i]] == matches[i]:
            dna_owner = row['name']
            if i == len(str_list) - 1:
                found = True
        else:
            dna_owner = 'No match'
            break
    if found:
        break

# print comparison result
print(dna_owner)

# close database
csv_file.close()