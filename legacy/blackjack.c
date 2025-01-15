#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "blackjack.h"

// Function to create a new deck
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
    const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

    if (card.rank >= 0 && card.suit >= 0) {
        printf("%s of %s\n", ranks[card.rank], suits[card.suit]);
    } else {
        printf("Invalid card\n");
    }
}

// Print the given hand. Also displays text. isPlayer acts as a boolean
void print_hand(Card* hand, int size, int isPlayer) {
    
    // Print out text to user
    if (isPlayer){  
        printf("--------- Player's Hand:\n");
    }
    else {
        printf("========= Dealer's Hand:\n");
    }

    //Print Hand
    for (int i = 0; i < size; i++) {
        print_card(hand[i]);
    }
}

Card* duplicate_deck(Card *original_deck, int size) {
    Card *new_deck = (Card *)malloc(size * sizeof(Card));
    if (new_deck == NULL) {
        perror("Failed to allocate memory for new deck");
        exit(EXIT_FAILURE);
    }
    memcpy(new_deck, original_deck, size * sizeof(Card));
    return new_deck;
}

int compare_cards(const void *a, const void *b) {
    Card *cardA = (Card *)a;
    Card *cardB = (Card *)b;

    return cardA->rank - cardB->rank;
}

int getHandValue(Card* hand, int no_cards, int* isSoft) {
    int value;
    int rank;
    int total = 0;

    //Purely for visualisation
    //Intuitively the cards should be printed in chronological order, so duplicate hand and then use this
    //to sort and calculate total. Leaves original hand unsorted and in chronological order
    // -- CAN REMOVE FOR SIMULATION BLACKJACK
    Card *copied_deck = duplicate_deck(hand, no_cards);

    //Quickly sort hand to reduce work later
    //Sorting always places Aces at the end as they have the largest value
    //Thus, will only need to check if total + 11 < 22 as it will be the last card.
    qsort(copied_deck, no_cards, sizeof(Card), compare_cards);

    //Sum up the hand
    for (int i = 0; i < no_cards; i++) {
        value = 0;
        rank = copied_deck[i].rank;
        if (rank < 9) {
            value = rank + 2;
        }
        // Conditional for ACE
        else if (rank == 12) {
            //Automatically default to 1 to prevent player from busting
            if (total + 11 < 22) {
                value = 11;
                *isSoft = 1;
            } else {
                value = 1;
            }
        } else { //All facecards are worth 10
            value = 10;
        }
        total += value;
    }

    free(copied_deck);
    return total;
}

void checkHand(Card* hand, int no_cards, int* outcome) {
    int total = 0;
    int isSoft = 0;
    
    total = getHandValue(hand, no_cards, &isSoft);

    //Now make final decision
    if (total == 21) {
        *outcome = 2;
    } else if (total > 21) {
        if (isSoft) {
            total -= 10;
            *outcome = -1;
            isSoft = 0;
            //Catch edge case where you dont continue playing because you hit blackjack
            if (total == 21) *outcome = 2;
            if (total > 21) *outcome = 0;
        } else {
            *outcome = 0;
        }
    } else {
        *outcome = -1;
    }
    //Must catch cases where the player can take ace as 1 or either without busting.
    if (isSoft && (total != 21)) {
        printf("Sir, your total is a soft %d\n", total);       
    } else {
        printf("Sir, your total is %d\n", total);
    }
}

void playBlackjack(Deck deck, double balance, int no_hands) {
    
    // Largest possible hand is 21 aces
    Card dealer_hand[21];
    // Once 5 cards is hit and the player has not busted they automatically win
    // MAYBE CHANGE AS A RULE FOR LATER 
    Card player_hand[5];

    double min_bet_amount = 20.0;
    double curr_bet_amount = 0.0;
    int validBet = 0;
    char decision;
    int no_cards_player;
    int no_cards_dealer;

    // -1 - still playing
    // 0 - loss
    // 1 - win
    // 2 - blackjack win
    int outcome = -1;
    int notStand = 1;

    int count = 0;
    while ((balance > min_bet_amount ) && (count < no_hands)){

        printf("------------ CURRENT BALANCE\n");
        printf("Crabb Coins: %f\n", balance);

        // Handles the player selecting an amount to wager
        while (!validBet) {
            printf("How much will you wager this round?\n");
            scanf("%lf", &curr_bet_amount);
            fflush(stdin);
            if (curr_bet_amount < min_bet_amount) {
                printf("I'm sorry sir.\nThe minimum bet for the table is %f\n", min_bet_amount);
            } 
            else if (curr_bet_amount > balance) {
                printf("I'm sorry sir.\nYou do not have the funds to place this bet.\n");
            }
            else {
                validBet = 1;
            }
        }
        validBet = 0;

        player_hand[0] = draw_card(&deck);
        dealer_hand[0] = draw_card(&deck);

        player_hand[1] = draw_card(&deck);

        //Manually Test Hands
        /*Card ace;   
        ace.rank = 12; // Ace = 12
        ace.suit = 1;  // Diamonds
        Card face;
        face.rank = 5;
        face.suit = 1;
        
        player_hand[0] = ace;
        player_hand[1] = face; */

        print_hand(player_hand, 2, 1);
        print_hand(dealer_hand, 1, 0);

        checkHand(player_hand, 2, &outcome);
        //printf("outcome is %d\n", outcome);
        int hasDoubled = 0;
        notStand = 1;
        no_cards_player = 2;
        while(outcome == -1 && notStand && !hasDoubled) {
            printf("Your action sir?\n H - hit, S - stand, D - double\n");
            scanf("%c", &decision);
            fflush(stdin);

            //Player decision cases
            switch(decision) {
                case 'H': //Hit
                    player_hand[no_cards_player] = draw_card(&deck);
                    no_cards_player += 1;
                    break;
                case 'D': //Double
                    
                    if (balance - 2 * curr_bet_amount < 0) {
                        printf("Unfortunately, you do not have the funds to double.");
                    } else {
                        curr_bet_amount += curr_bet_amount;
                        printf("You only get 1 card\n");
                        player_hand[no_cards_player] = draw_card(&deck);
                        no_cards_player += 1;
                        hasDoubled = 1;

                    }
                    break;
                case 'S': //Stand
                    notStand = 0;
                    break;
                //NEED TO IMPLEMENT SPLIT 
                default:
                    printf("You stupid\n");
            }

            if (notStand || hasDoubled) {
                checkHand(player_hand, no_cards_player, &outcome);
                print_hand(player_hand, no_cards_player, 1);

                //If the player has 5 cards and has not busted
                if (no_cards_player == 5 && outcome != 0) {
                    printf("5 Card Charlie!\n");
                    outcome = 1;
                }
            }
        }

        //Only deal cards to dealer if player has not busted or won blackjack as these exit the hand
        no_cards_dealer = 1;
        if (outcome == -1) {
            // Keep drawing until dealer busts or gets 17   
            // SOFT 17 dealer keeps drawing so need to update    
            while (getHandValue(dealer_hand, no_cards_dealer, &notStand) < 17) {
                dealer_hand[no_cards_dealer] = draw_card(&deck);
                no_cards_dealer += 1;
                printf("Dealer draws a card!\n");
                print_hand(dealer_hand, no_cards_dealer, 0);
                printf("Value %d\n", getHandValue(dealer_hand, no_cards_dealer, &notStand));
            }
            int dealer_value = getHandValue(dealer_hand, no_cards_dealer, &notStand);
            int player_value = getHandValue(player_hand, no_cards_player, &notStand);
            if (dealer_value > 21) {
                printf("Dealer Busts!\n");
                outcome = 1;
            } else if (dealer_value > player_value) {
                printf("Unfortunately sir the dealer's %d beats your %d!\n", dealer_value, player_value);
                outcome = 0;
            } else if (dealer_value == player_value) { //NOT ALL CASINOS USE THIS 
                printf("Stand Off\n");
                outcome = 3;
            }
            else {
                printf("Congratulations sir your %d beats the dealer's %d\n", player_value, dealer_value);
                outcome = 1;
            }

        }

        //Update balance
        switch (outcome){
            case 0:
                printf("Unfortunately sir, you lose.\n");
                balance -= curr_bet_amount;
                break;
            case 1:
                printf("Congratulations sir, you win\n");
                balance += curr_bet_amount;
                break;
            case 2:
                printf("BLACKJACK! Well done sir\n");
                balance += curr_bet_amount * BLACKJACK_RATIO;
                break;
            case 3:
                printf("No one wins\n");
                break;
        }

        count++;
    }
}

/*void simBlackjack(Deck deck, float start_balance, int no_hands) {
    
    Card card = draw_card(&deck);

    int count = 0;
    while (start_balance > 0 && count < no_iterations) {
         
    }

    

}*/

