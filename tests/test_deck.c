#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../core/deck.h"

void test_create_deck() {
    //Generates in order
    Deck test_deck = create_deck();
    for (int i = 0; i < DECK_SIZE; i++) {
        assert(test_deck.cards[i].rank == i % NUM_RANKS);
        assert(test_deck.cards[i].suit == i / NUM_RANKS);
    }
    printf("test_create_deck passed.\n");
}

void test_draw_card() {
    Deck test_deck = create_deck();
    for (int i = 0; i < DECK_SIZE; i++) {
        Card c = draw_card(&test_deck);

        //Visualise Cards
        //print_card(c);

        assert(c.rank == i % NUM_RANKS);
        assert(c.suit == i / NUM_RANKS);
        
    }
    //Once empty, deck should return invalid card [-1,-1]
    Card c = draw_card(&test_deck);
    assert(c.rank == -1 && c.suit == -1);  
    printf("test_draw_card passed.\n");
}

void test_shuffle_deck() {
    Deck test_deck = create_deck();
    Deck original_deck = create_deck();
    
    shuffle_deck(&test_deck);

    int is_different = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (original_deck.cards[i].rank != test_deck.cards[i].rank ||
            original_deck.cards[i].suit != test_deck.cards[i].suit) {
            is_different = 1;
            break;
        }
    }
    assert(is_different == 1);
    printf("test_shuffle_deck passed.\n");
}

int main() {
    printf("Running tests...\n");
    test_create_deck();
    test_draw_card();
    test_shuffle_deck();
    printf("All tests passed successfully!\n");
    return 0;
}
