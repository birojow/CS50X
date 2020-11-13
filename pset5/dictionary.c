// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of words loaded into the hash table
int total_words = 0;

// Number of buckets in hash table
const unsigned int N = 186023;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word) {
    // lowercase the word for comparison
    char lower_case_word[LENGTH + 1];
    for (int i = 0; i < LENGTH + 1; i++) {
        lower_case_word[i] = ' ';
    }

    for (int j = 0; j < LENGTH + 1; j++) {
        if (word[j] == 0) {
            lower_case_word[j] = 0;
            break;
        }
        lower_case_word[j] = tolower(word[j]);
    }

    // hash the lowercased word
    unsigned int hash_code = hash(lower_case_word);
    if (table[hash_code] == NULL) {
        // empty bucket
        return false;
    }
    else {
        // iterate through the list searching for the word
        node *cursor = table[hash_code];
        while (1 == 1) {
            int is_equal = strcasecmp(cursor->word, word);
            if (is_equal == 0) {
                return true;
            }
            else {
                if (cursor->next == NULL) {
                    break;
                }
                cursor = cursor->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) {
    // I'm just multiplying each letter for 31, a prime number
    // and multiplying again for the letter position
    // my own hash function, thank you
    unsigned int hash_code = 0;
    for (int i = 0; i < LENGTH + 1; i++) {
        if (word[i] == 0) {
            break;
        }
        hash_code += word[i] * 31 * i;
    }
    if (hash_code > N) {
        hash_code = hash_code % N;
    }
    return hash_code;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary) {
    // open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL) {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // initialize all letters from the buffer to space, just in case
    char word[LENGTH + 1];
    for (int i = 0; i < LENGTH + 1; i++) {
        word[i] = ' ';
    }

    // read strings from file one at a time
    while (1 == 1) {
        int result = fscanf(dict, "%s", word);

        // if end of file, end function
        if (result == EOF) {
            fclose(dict);
            return true;
        }

        // if not EOF, create a new node
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL) {
            printf("Could not allocate enough memory.\n");
            return false;
        }

        // initialize the values of the node
        strcpy(new_word->word, word);
        new_word->next = NULL;

        // hash word to obtain a hash value
        unsigned int hash_code = hash(word);

        // insert node into hash table at that location
        new_word->next = table[hash_code];
        table[hash_code] = new_word;
        total_words++;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void) {
    return total_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void) {
    // just iterate through the table, freeing each node
    for (int i = 0; i < N; i++) {
        while (table[i] != NULL) {
            node *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
        if (i == N - 1) {
            return true;
        }
    }
    return false;
}
