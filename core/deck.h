#ifndef DECK_H
#define DECH_H

#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)

//Hand Structure
typedef struct {
    int rank;
    int suit;
} Card;

// Deck Structure
typedef struct {
    Card cards[DECK_SIZE];
    int top_card;
} Deck;

// ----- FUNCTIONS
Deck create_deck();
void shuffle_deck(Deck *deck);
Card draw_card(Deck *deck);
Card* duplicate_deck(Card *original_deck, int size);

#endif`