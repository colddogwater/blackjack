#ifndef BLACKJACK_H
#define BLACKJACK_H

#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)

// -- SPECIFIC RULES -- 
#define BLACKJACK_RATIO 1.5  // Payout ratio 3:2 = 1.5

// Card structure
typedef struct {
    int rank;
    int suit;
} Card;

// Deck structure
typedef struct {
    Card cards[DECK_SIZE];
    int top_card; 
} Deck;


// Function declarations
Deck create_deck();
void shuffle_deck(Deck *deck);
Card draw_card(Deck *deck);
void print_card(Card card);
void print_hand(Card* hand, int size, int isPlayer);
Card* duplicate_deck(Card *original_deck, int size);
int compare_cards(const void *a, const void *b);
int getHandValue(Card* hand, int no_cards, int* isSoft);
void playBlackjack(Deck deck, double start_balance, int no_hands);

#endif
