#include "cs50.h"
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct {
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int winner, int current_loser);

int main(int argc, string argv[]) {
    // Check for invalid usage
    if (argc < 2) {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX) {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++) {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) {
    // check if candidate exists
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(name, candidates[i]) == 0) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
    // iterate over ranks
    for (int i = 0; i < candidate_count - 1; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            // just checking if i wins over j,
            // or j wins over i
            if (preferences[i][j] > preferences[j][i]) {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i]) {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // bubble sort
    for (int iterations = pair_count - 1; iterations > 0; iterations--) {
        for (int i = 0; i < pair_count - 1; i++) {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser]) {
                pair temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
    for (int p = 0; p < pair_count; p++) {   
        if (check_cycle(pairs[p].winner, pairs[p].loser)) {
            continue;
        }
        locked[pairs[p].winner][pairs[p].loser] = true;
    }
    return;
}

// Print the winner of the election
void print_winner(void) {
    for (int i = 0; i < candidate_count - 1; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            // if candidate is locked, go to next
            if (locked[j][i]) {
                break;
            }
            // if not locked, print as winner, and go to next
            if (j == candidate_count - 1) {
                printf("%s\n", candidates[i]);
            }
        }
    }
    return;
}

// Recursively checks if a cycle will be created
bool check_cycle(int winner, int current_loser) {
    // winner can be traced to himself
    // a.k.a. CYCLE
    if (winner == current_loser) {
        return true;
    }

    // iterate over all candidates
    for (int loser = 0; loser < candidate_count; loser++) {
        if (current_loser == loser) {
            continue;
        }
        // iterating over all losers of the current loser
        if (locked[current_loser][loser]) {
            bool cycle = check_cycle(winner, loser);
            if (cycle) {
                return true;
            }
        }
    }
    return false;
}