#include <stdio.h>
#include "../core/hand.h"
#include "../core/deck.h"

void test_init_hand() {
    Hand player_hand = init_hand(0);
    if (player_hand.no_cards == 0 && player_hand.total == 0 && player_hand.status == 0) {
        printf("PASS: Hand initialized correctly.\n");
    } else {
        printf("FAIL: Hand initialization failed.\n");
    }
}

void test_add_card_to_hand() {
    Hand player_hand = init_hand(0);

    Card card1 = {9, 0};  // Jack of Hearts
    add_card_to_hand(&player_hand, card1);

    if (get_hand_value(&player_hand) == 10 && player_hand.no_cards == 1) {
        printf("PASS: Adding a single card works.\n");
    } else {
        printf("FAIL: Adding a single card failed.\n");
    }

    Card card2 = {12, 1};  // Ace of Diamonds 
    add_card_to_hand(&player_hand, card2);

    if (get_hand_value(&player_hand) == 21) {
        printf("PASS: Adding an Ace correctly updates total.\n");
    } else {
        printf("FAIL: Adding an Ace incorrectly handled.\n");
    }

}

void test_bust_hand() {
    Hand player_hand = init_hand(0);
 
    // Difference is two; i.e. 5 = 7 of clubs, 9 = 11 = Jack
    Card card1 = {9, 0};  // Jack of Hearts
    Card card2 = {9, 1};  // Jack of Diamonds
    Card card3 = {5, 2};  // 7 of Clubs

    add_card_to_hand(&player_hand, card1);
    add_card_to_hand(&player_hand, card2);
    add_card_to_hand(&player_hand, card3);

    //print_hand(&player_hand);
    if (player_hand.status == -1) {
        printf("PASS: Bust condition detected correctly.\n");
    } else {
        printf("FAIL: Bust condition not detected.\n");
    }
}

void test_blackjack_hand() {
    Hand player_hand = init_hand(0);

    Card card1 = {9, 0};  // Jack of Hearts
    Card card2 = {12, 2}; // Ace of Clubs

    add_card_to_hand(&player_hand, card1);
    add_card_to_hand(&player_hand, card2);

    //print_hand(&player_hand);
    if (player_hand.status == 3) {
        printf("PASS: Blackjack detected correctly.\n");
    } else {
        printf("FAIL: Blackjack detection failed.\n");
    }
}

void test_hand_value_calculation() {
    Hand player_hand = init_hand(0);

    Card card1 = {5, 0};  // 7 of Hearts
    Card card2 = {10, 1}; // Queen of Diamonds

    add_card_to_hand(&player_hand, card1);
    add_card_to_hand(&player_hand, card2);

    //print_hand(&player_hand);
    if (get_hand_value(&player_hand) == 17) {
        printf("PASS: Hand value calculated correctly.\n");
    } else {
        printf("FAIL: Hand value calculation is incorrect.\n");
    }
}

void test_ace_edge_cases() {
    Hand player_hand = init_hand(0);

    //=========== EDGE CASE #1 - Two Aces
    Card card1 = {12, 0};  // Ace of Hearts
    Card card2 = {12, 1}; // Ace of Diamonds

    add_card_to_hand(&player_hand, card1);
    add_card_to_hand(&player_hand, card2);

    //print_hand(&player_hand);
    if (get_hand_value(&player_hand) == 12) {
        printf("PASS: Ace edge case - Two Aces. \n");
    } else {
        printf("FAIL: Ace edge case - Two Aces.\n");
    }

    player_hand = init_hand(0);

    //=========== EDGE CASE #2 - Old Aces gets converted
    Card card3 = {6, 0};  // 8 of Hearts
    Card card4 = {12, 1}; // Ace of Diamonds
    Card card5 = {12, 0};  // Ace of Hearts
    Card card6 = {9, 1}; // Jack of Diamonds

    add_card_to_hand(&player_hand, card3);
    add_card_to_hand(&player_hand, card4);
    add_card_to_hand(&player_hand, card5);
    add_card_to_hand(&player_hand, card6);

    //print_hand(&player_hand);
    if (get_hand_value(&player_hand) == 20) {
        printf("PASS: Ace edge case - Sandwiched Aces.\n");
    } else {
        printf("FAIL: Ace edge case - Sandwiched Aces.\n");
    }

    player_hand = init_hand(0);

    //=========== EDGE CASE #2 - All Aces
    Card card7 = {12, 1}; // Ace of Diamonds
    Card card8 = {12, 1}; // Ace of Diamonds
    Card card9 = {12, 1}; // Ace of Diamonds
    Card card10 = {12, 1}; // Ace of Diamonds
    Card card11 = {12, 1}; // Ace of Diamonds
    Card card12 = {12, 1}; // Ace of Diamonds
    Card card13 = {12, 1}; // Ace of Diamonds
    Card card14 = {12, 1}; // Ace of Diamonds
    Card card15 = {12, 1}; // Ace of Diamonds
    Card card16 = {12, 1}; // Ace of Diamonds
    Card card17 = {12, 1}; // Ace of Diamonds

    add_card_to_hand(&player_hand, card7);
    add_card_to_hand(&player_hand, card8);
    add_card_to_hand(&player_hand, card9);
    add_card_to_hand(&player_hand, card10);
    add_card_to_hand(&player_hand, card11);
    add_card_to_hand(&player_hand, card12);
    add_card_to_hand(&player_hand, card13);
    add_card_to_hand(&player_hand, card14);
    add_card_to_hand(&player_hand, card15);
    add_card_to_hand(&player_hand, card16);

    //print_hand(&player_hand);
    if (get_hand_value(&player_hand) == 20 && get_hand_status(&player_hand) == 0) {
        printf("PASS: Ace edge case - All Aces 1. \n");
    } else {
        printf("FAIL: Ace edge case - All Aces 1.\n");
    }

    //Final Ace will make blackjack and 21
    add_card_to_hand(&player_hand, card17);

    //print_hand(&player_hand);
    if (get_hand_value(&player_hand) == 21 && get_hand_status(&player_hand) == 3) {
        printf("PASS: Ace edge case - All Aces 2.\n");
    } else {
        printf("FAIL: Ace edge case - All Aces 2.\n");
    }
}

int main() {
    printf("Running hand tests...\n");

    test_init_hand();
    test_add_card_to_hand();
    test_bust_hand();
    test_blackjack_hand();
    test_hand_value_calculation();
    test_ace_edge_cases();

    printf("Hand tests completed.\n");
    return 0;
}
