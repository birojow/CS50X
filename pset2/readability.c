#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void count_letters_words_and_sentences(string text);
void calculate_grade(float letters, float words, float sentences);
void print_grade(int index);

int main(void) {
    string text = get_string("Text: ");
    count_letters_words_and_sentences(text);
}

void count_letters_words_and_sentences(string text) {
    float letters = 0;
    float spaces = 0;
    float sentences = 0;
    for (int i = 0; i < strlen(text); i++) {
        if (text[i] == ' ') {
            spaces++;
        }
        if (text[i] == '.' || text[i] == '?' || text[i] == '!') {
            sentences++;
        }
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
            letters++;
        }
    }
    int words = spaces + 1;
    calculate_grade(letters, words, sentences);
}

void calculate_grade(float letters, float words, float sentences) {
    float lettersper100words = (letters / words) * 100;
    float sentencesper100words = (sentences / words) * 100;
    int index = round(0.0588 * lettersper100words - 0.296 * sentencesper100words - 15.8);
    print_grade(index);
}

void print_grade(int index) {
    if (index < 1) {
        printf("Before Grade 1\n");
    }
    else if (index >= 1 && index <= 15) {
        printf("Grade %i\n", index);
    }
    else {
        printf("Grade 16+\n");
    }
}