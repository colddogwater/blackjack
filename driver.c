#include "blackjack.h"

// ---------------------- DRIVER CODE -------------------------
int main() {
    //User changeable variables
    int no_hands = 5;
    double starting_balance = 100.0;
    
    // Create a new deck
    Deck deck = create_deck();

    // Shuffle the deck
    shuffle_deck(&deck);

    playBlackjack(deck, starting_balance, no_hands);

    return 0;
}
