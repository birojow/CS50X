import cs50
import csv
from sys import argv, exit

# check number of arguments
if len(argv) != 2:
    print("Usage: python import.py data.csv")
    exit(1)

# set filepaths
filepath = argv[1]
db = cs50.SQL("sqlite:///students.db")

# open csv file and import houses, birth and full names into lists
houses = []
births = []
full_names = []
with open(filepath, "r") as students_data:
    reader = csv.DictReader(students_data)
    for row in reader:
        full_names.append(row['name'].split())
        houses.append(row['house'])
        births.append(row['birth'])

# splitting full names into first, middle and last names
first_names = []
middle_names = []
last_names = []
for name in full_names:
    first_names.append(name[0])
    if len(name) == 2:
        middle_names.append(None)
        last_names.append(name[1])
    else:
        middle_names.append(name[1])
        last_names.append(name[2])

# inserting data into the database
for i in range(len(first_names)):
    db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
               first_names[i], middle_names[i], last_names[i], houses[i], births[i])