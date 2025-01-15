#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

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

// Duplicates the deck
Card* duplicate_deck(Card *original_deck, int size) {
    Card *new_deck = (Card *)malloc(size * sizeof(Card));
    if (new_deck == NULL) {
        perror("Failed to allocate memory for new deck");
        exit(EXIT_FAILURE);
    }
    memcpy(new_deck, original_deck, size * sizeof(Card));
    return new_deck;
}