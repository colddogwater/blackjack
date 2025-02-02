#include "game.h"
#include "hand.h"
#include "deck.h"
#include <stdio.h>

// Initiliase everything
void start_game(GameState *game){
    game->deck = create_deck();
    shuffle_deck(&game->deck);

    game->player = init_hand(0);
    game->dealer = init_hand(1);
    game->balance = 150;
}

//Think this can be replaced with a function wrapper later
// Also maybe need to add input safeties
float get_player_bet(float current_balance) {
    float wager;
    int invalid = 1;
    while(invalid) {
        printf("How much would you like to wager? (Current balance: %2.f)\n", current_balance);
        scanf("%f", &wager);
        if (wager > current_balance) {
            printf("You do not have the funds to wager that amount!\n");
        } else {
            invalid = 0;
        }
    }
    
    return wager;
}

void deal_initial_hands(GameState *game) {
    add_card_to_hand(&game->player, draw_card(&game->deck));
    add_card_to_hand(&game->player, draw_card(&game->deck));
    add_card_to_hand(&game->dealer, draw_card(&game->deck));
}

//Draw till over 17
void dealer_draw(GameState *game) {
    while (get_hand_value(&game->dealer) < 17) {
        add_card_to_hand(&game->dealer, draw_card(&game->deck));
    }
}

int check_winner(GameState *game) {
    int player_value = get_hand_value(&game->player);
    int dealer_value = get_hand_value(&game->dealer);

    if (player_value > 21) return -1;  // Player busts
    if (dealer_value > 21) return 1;   // Dealer busts
    if (player_value > dealer_value) return 1;  // Player wins
    if (player_value < dealer_value) return -1; // Dealer wins
    return 0;  // Draw
}