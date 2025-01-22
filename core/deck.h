#include <string.h>
#include <time.h>

#ifndef DECK_H
#define DECK_H

#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)

//Card Structure
typedef struct {
    int rank;
    int suit;
} Card;

// Deck Structure
typedef struct {
    Card cards[DECK_SIZE];
    int top_card;
} Deck;

extern const char *RANK_NAMES[NUM_RANKS];
extern const char *SUIT_NAMES[NUM_SUITS];

// ----- FUNCTIONS
Deck create_deck();
void shuffle_deck(Deck *deck);
Card draw_card(Deck *deck);
void print_card(Card card);

#endif