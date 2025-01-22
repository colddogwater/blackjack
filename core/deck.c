#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

//ITOS arrays for rank and suit
const char *RANK_NAMES[NUM_RANKS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
const char *SUIT_NAMES[NUM_SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};

// Create a new deck in order
Deck create_deck() {
    Deck deck;
    deck.top_card = 0; // Initialize the top card index

    for (int suit = 0; suit < NUM_SUITS; ++suit) {
        for (int rank = 0; rank < NUM_RANKS; ++rank) {
            deck.cards[suit * NUM_RANKS + rank].rank = rank;
            deck.cards[suit * NUM_RANKS + rank].suit = suit;
        }
    }
    return deck;
}

// Function to shuffle the deck
void shuffle_deck(Deck *deck) {
    srand(time(NULL)); // Seed the random number generator

    for (int i = DECK_SIZE - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

// Function to draw a card from the deck
Card draw_card(Deck *deck) {
    if (deck->top_card < DECK_SIZE) {
        return deck->cards[deck->top_card++];
    } else {
        // Return an invalid card if the deck is empty
        Card invalid_card = {-1, -1};
        return invalid_card;
    }
}

// Function to print a card
void print_card(Card card) {
    if (card.rank >= 0 && card.suit >= 0) {
        printf("%s of %s\n", RANK_NAMES[card.rank], SUIT_NAMES[card.suit]);
    } else {
        printf("Invalid card\n");
    }
}