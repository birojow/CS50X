import cs50
from sys import argv, exit

# check number of arguments
if len(argv) != 2:
    print("Usage: python roster.py house name")
    exit(1)
house = argv[1]

# set the database filepath
db = cs50.SQL("sqlite:///students.db")

# realizes the query
data = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

# iterate over data, printing it
for person in data:
    if person['middle'] == None:
        print(f"{person['first']} {person['last']}, born in {person['birth']}")
    else:
        print(f"{person['first']} {person['middle']} {person['last']}, born in {person['birth']}")