#include <stdio.h>
#include "../core/game.h"

int start_cli_game() {
    GameState game;
    float wager;
    char input;
    int busted = 0;
    float balance = 150.0;
    int IS_DOUBLED;

    start_game(&game);

    //Game Loop
    while (balance > 0) {
        IS_DOUBLED = 0;
        start_game(&game);
        busted = 0;

        //Wager
        wager = get_player_bet(balance);
        //Get cards
        deal_initial_hands(&game);
        //Decision loop
        while (game.player.status == 0) {
            printf("Sir, your cards,\n");
            print_hand(&game.player);
            printf("Dealer is showing, \n");
            print_hand(&game.dealer);
            printf("Hit (H), Stand (S), or Double (D)?\n");
            scanf("%c", &input);
            //The add card and print hand can probs be added to a function with a
            // general function wrapper to increase re-usability
            if(input == 'H') {
                add_card_to_hand(&game.player, draw_card(&game.deck));
                print_hand(&game.player);
                if (get_hand_value(&game.player) < 22){
                    
                } else if (get_hand_value(&game.player) == 21) {//NEED TO ADD INSTANT PAYOUT HERE
                    printf("21!\n");
                    game.player.status = 3;
                } else { // Busted
                    busted = 1;
                }
            } else if (input == 'S') {
                game.player.status = 1;
            } else if (input == 'D') {
                if (balance -  2*wager > 0 ) {
                    printf("Drawing one card...\n");
                    add_card_to_hand(&game.player, draw_card(&game.deck));
                    print_hand(&game.player);
                    game.player.status = 2;
                    IS_DOUBLED = 1;
                } else {
                    printf("Insufficient Funds to Double!\n");
                }
            }
        }

        if (!busted)
            dealer_draw(&game);
            printf("Dealer draws to,\n");
            print_hand(&game.dealer);

        //Evaluate winner and payout only if player hasn't busted
        if (check_winner(&game) == 1 && !busted) {
            printf("Winner!\n");
            wager = wager + (wager * IS_DOUBLED);
        } else if (check_winner(&game) == 0 && !busted){
            printf("Standoff!\n");
            wager = 0;
        } else {
            printf("You lose...\n");
            wager = -wager - (wager * IS_DOUBLED);
        }

        balance += wager;
    }

    return 0;
}