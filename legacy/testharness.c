#include "blackjack.h"
#include <stdio.h>

void testCondition(int condition) {
    if (condition) {
        printf("TEST PASSED\n");
    } else {
        printf("TEST FAILED\n");
    }
}

int main () {

    // Create a new deck
    Deck deck = create_deck();

    // Shuffle the deck
    shuffle_deck(&deck);

    //---------------- getHandValue Tests
    Card ace;   
    ace.rank = 12; // Ace = 12
    ace.suit = 1;  // Diamonds
    Card face;
    face.rank = 10;
    face.suit = 1;
    int dummy = 0;

    //Max for player is 5 (5 card charlie)
    Card player_hand[5];

    //Max for dealer is 17 (17 aces)
    Card dealer_hand[12];
    
    player_hand[0] = ace;
    player_hand[1] = face; 
    testCondition(getHandValue(player_hand, 2 , &dummy) == 21);

    return 1;
}