from cs50 import get_string

text = get_string('Text: ')

letters = 0
spaces = 0
sentences = 0

for character in text:
    if (character >= 'a' and character <= 'z') or (character >= 'A' and character <= 'Z'):
        letters += 1
    if character == ' ':
        spaces += 1
    if character == '.' or character == '!' or character == '?':
        sentences += 1

words = spaces + 1
letters_per_100_words = (letters / words) * 100
sentences_per_100_words = (sentences / words) * 100
cl_index = round(0.0588 * letters_per_100_words - 0.296 * sentences_per_100_words - 15.8)

if cl_index < 1:
    print('Before Grade 1')
elif cl_index < 16:
    print('Grade ' + str(cl_index))
else:
    print('Grade 16+')