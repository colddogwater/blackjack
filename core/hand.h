#include <string.h>
#include <time.h>
#include "deck.h"

#ifndef HAND_H
#define HAND_H

//Hand Structure
typedef struct {
    Card cards[11];
    int no_cards;
    int total;
    int status;
    // Status
    // 3 - Blackjack
    // 2 - Doubled
    // 1 - Standing
    // 0 - Still Playing
    // -1 - Busted
    int hand_type;
    // Hand Types
    // 0 - Player
    // 1 - Dealer
} Hand;

Hand init_hand(int hand_Type);
void add_card_to_hand(Hand *hand, Card card);
int get_hand_status(const Hand *hand);
int get_hand_value(const Hand *hand);
void print_hand(Hand* hand);


#endif