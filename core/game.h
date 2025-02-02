#include "deck.h"
#include "hand.h"

#ifndef GAME_H
#define GAME_H

typedef struct {
    Deck deck;
    Hand player; // Maybe array for splitting
    Hand dealer;
    int game_over;
    float balance;
} GameState;

void start_game(GameState *game);
float get_player_bet(float current_balance);
void deal_initial_hands(GameState *game);
void dealer_draw(GameState *game);
int check_winner(GameState *game);

#endif