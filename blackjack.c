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

//I think you could also have the card struct carry a value and then check that but maybe will explore that later
int checkIfSplittable(Card* hand) {
    Card card_1 = hand[0];
    Card card_2 = hand[1];

    //Catches if they are identical cards avoids next processing
    if (card_1.rank == card_2.rank)
        return 1;

    //Must catch faces cards which allow doubling but have different ranks
    //If neither card is an ace then the face cards will be highest value and we can simply look at their sums
    //Double aces will be caught with above if statement too so mustnt worry
    // 9 | 10 | J |  Q |  K
    // 7 |  8 | 9 | 10 | 11
    //Thus 10 + 10 = 16 and K + K = 22
    if (card_1.rank != 12 || card_2.rank != 12) 
        if (card_1.rank + card_2.rank <= 22 && card_1.rank + card_2.rank >= 16) 
            return 1;
    
    return 0;
}

void playHand(Deck deck, Hand* all_hands, int hand_index, double curr_bet_amount, double balance, int* outcome){
    char decision;
    int no_cards_player = 2;
    all_hands[hand_index].stillPlaying = 1; 

    //Catches the recursive split call and automatically draws another card
    //Must also catch here a blackjack straight from first hit
    if (hand_index != 0) {
        printf("Your first hit for the split sir\n");
        all_hands[hand_index].hands[1] = draw_card(&deck);
        print_hand(all_hands[hand_index].hands, no_cards_player, 1);
        checkHand(all_hands[0].hands, 2, &outcome);
        if (&outcome == 2 && INSTANT_BLACKJACK_PAYOUT) {
            all_hands[hand_index].stillPlaying = 0;
        }
    }
    
    while(all_hands[hand_index].stillPlaying) {
        printf("Your action sir?\n H - hit, S - stand, D - double, X - Split\n");
        scanf("%c", &decision);
        fflush(stdin);

        //Player decision cases
        switch(decision) {
            case 'H': //Hit
                all_hands[hand_index].hands[no_cards_player] = draw_card(&deck);
                no_cards_player += 1;
                break;
            case 'D': //Double
                
                if (balance - 2 * curr_bet_amount < 0) {
                    printf("Unfortunately, you do not have the funds to double.");
                } else {
                    curr_bet_amount += curr_bet_amount;
                    printf("You only get 1 card\n");
                    all_hands[hand_index].hands[no_cards_player] = draw_card(&deck);
                    no_cards_player += 1;
                    all_hands[hand_index].stillPlaying = 0;
                }
                break;
            case 'S': //Stand
                all_hands[hand_index].stillPlaying = 0;
                break;
            case 'X':
                //Must catch splitting face cards as they all have equal value
                if (checkIfSplittable(all_hands[hand_index].hands))
                    if (hand_index > MAX_ALLOWED_SPLITS)
                        printf("I'm sorry sir but we do not allow more splitting\n");
                    else {
                        printf("Splitting\n");
                        
                        // Give one of the cards to the new hand
                        all_hands[hand_index + 1].hands[0] = all_hands[hand_index + 1].hands[1];
                        no_cards_player--;

                        playHand(deck, all_hands, hand_index + 1, curr_bet_amount, balance, &outcome);
                    }
                else
                    printf("NOT SPLITTABLE\n");
                break;
            default:
                printf("You stupid\n");
        }

        if (all_hands[hand_index].stillPlaying) {
            checkHand(all_hands[hand_index].hands, no_cards_player, &outcome);
            print_hand(all_hands[hand_index].hands, no_cards_player, 1);

            //If the player has 5 cards and has not busted
            if (no_cards_player == 5 && &outcome != 0) {
                printf("5 Card Charlie!\n");
                printf("Hi littlet\n");
                *outcome = 1; 
                printf("segment XDDD\n");
            }
        }
    }
}

void playRound(Deck deck,  Hand* all_hands, double curr_bet_amount, double balance, int* outcome) {
    
    // Largest possible hand is 21 aces
    Card dealer_hand[21];
    // Once 5 cards is hit and the player has not busted they automatically win
    // MAYBE CHANGE AS A RULE FOR LATER 
    //Card player_hand[5];
    int no_hands = 0;
    
    all_hands[0].hands[0] = draw_card(&deck);
    all_hands[0].hands[1] = draw_card(&deck);
    no_hands++;
   
    dealer_hand[0] = draw_card(&deck);

    //Manually Test Hands
    /*Card ace;   
    ace.rank = 9; // Ace = 12
    ace.suit = 1;  // Diamonds
    Card face;
    face.rank = 8;
    face.suit = 2;
    
    player_hand[0] = ace;
    player_hand[1] = face; */

    print_hand(all_hands[0].hands, 2, 1);
    print_hand(dealer_hand, 1, 0);

    //for outcome
    // -1 - still playing
    // 0 - loss
    // 1 - win
    // 2 - blackjack win
    checkHand(all_hands[0].hands, 2, &outcome);
    if (!(&outcome == 2 && INSTANT_BLACKJACK_PAYOUT)) {
        playHand(deck, all_hands, 0, curr_bet_amount, balance, &outcome);
    }

    /*
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

    }*/


} 

void playBlackjack(Deck deck,  double balance, int no_hands) {

    double curr_bet_amount = 0.0;
    int validBet = 0;
    char decision;

    // -1 - still playing
    // 0 - loss
    // 1 - win
    // 2 - blackjack win
    int outcome = -1;
    int notStand = 1;
    int total_splits = 0;

    Hand all_hands[MAX_ALLOWED_SPLITS];
    int hand_tot = 0;

    int count = 0;
    while ((balance > MIN_BET_ALLOWED ) && (count < no_hands)){

        printf("------------ CURRENT BALANCE\n");
        printf("Crabb Coins: %f\n", balance);

        // Handles the player selecting an amount to wager
        while (!validBet) {
            printf("How much will you wager this round?\n");
            scanf("%lf", &curr_bet_amount);
            fflush(stdin);
            if (curr_bet_amount < MIN_BET_ALLOWED) {
                printf("I'm sorry sir.\nThe minimum bet for the table is %f\n", MIN_BET_ALLOWED);
            } 
            else if (curr_bet_amount > balance) {
                printf("I'm sorry sir.\nYou do not have the funds to place this bet.\n");
            }
            else {
                validBet = 1;
            }
        }
        validBet = 0;

        playRound(deck, all_hands, curr_bet_amount, balance, &outcome);
        break;
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

