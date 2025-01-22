#include <stdio.h>
#include "deck.h"
#include "hand.h"

const int RANK_VALUES[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

//Initiliase Hand
Hand init_hand(int hand_Type) {
    Hand hand = {0};
    
    hand.hand_type = hand_Type;

    return hand;
}

void add_card_to_hand(Hand *hand, Card card) {
    if (hand->no_cards < 11) {
        hand->cards[hand->no_cards] = card;
        hand->no_cards += 1;
        hand->status = get_hand_status(hand);

    } else {
        printf("Hand is full!\n");
    }
}

//Get Hand Status
//Can only determine still playing or busted, otherwise the input needs to
//change these values as they play
int get_hand_status(const Hand *hand) {
    int hand_value = get_hand_value(hand);

    int status = 0;

    if (hand_value > 21) {
        //Busted
        status = -1;  
    } else if (hand_value == 21) {
        //Hit Blackjack
        status = 3;  
    }
    return status;
}

int get_hand_value(const Hand *hand) {
    int hand_value = 0;
    int ace = 0; //False
    
    for (int i=0; i < hand->no_cards; i++) {
        //If first card is an Ace
        if (hand->cards[i].rank == 12) {
            hand_value += 1;
            ace = 1;
        } else {
            hand_value += RANK_VALUES[hand->cards[i].rank];
        }
    }
    //All Aces are defaulted to 1, if not bust change to 11
    if (ace && hand_value + 10 < 22) {
        hand_value += 10;
    }

    return hand_value;
}

void print_hand(Hand* hand) {
    //Print Hand
    for (int i = 0; i < hand->no_cards; i++) {
        print_card(hand->cards[i]);
    }
    //May need to print in order or something, adding a card
    // fucks the chronological order of printing so it looks bad -- otherwise works
}

