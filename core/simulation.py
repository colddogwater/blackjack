import random
from core import rules
from core.logic import get_hand_value

def run_simulation(decision_chart, no_steps):
    balance = 0
    wager = 1

    player_cards = []
    dealer_cards = []

    discard_pile = []

    DECK = 4 * ['2', '3', '4', '5', '6', '7', '8', '9', 'J', 'J', 'J', 'J', 'A'] * rules.NO_DECKS
    random.shuffle(DECK)
    
    for x in range(no_steps):
        #print("---------------------------------")
        player_cards.extend(DECK.pop())
        player_cards.extend(DECK.pop())

        dealer_cards.extend(DECK.pop())

        #print("Player hand: " + str(player_cards) + "Dealer hand: " + str(dealer_cards))
        #print("Cards in deck: " + str(len(DECK)))

        win_amount = game_loop(player_cards, dealer_cards, DECK, decision_chart)

        balance += win_amount * wager 
        print("Balance, " + str(balance))

        #user_input = input("Please enter something: ")

        discard_pile.extend(player_cards)
        discard_pile.extend(dealer_cards)
        
        player_cards.clear()
        dealer_cards.clear()

        #print(str(discard_pile))
        #print("Cards in shoe: " + str(len(discard_pile)))

        # If length of discard pile is bigger than 20% of the pile reshuffle 
        if len(discard_pile) > (0.2 * 52 * rules.NO_DECKS):
            #print("Reshuffling")
            DECK.extend(discard_pile)
            random.shuffle(DECK)
            discard_pile.clear()

        player_cards = []
        dealer_cards = []


def game_loop(player_cards, dealer_cards, DECK, decision_chart):
    win_amount = None

    while win_amount == None:
        #print("Player cards: " + str(player_cards))
        hand_value = get_hand_value(player_cards)
        if hand_value == 21 and rules.INSTANT_PAYOUT:
            win_amount = rules.BJ_PAYOUT
        elif hand_value > 21:
            win_amount = -1
        elif len(player_cards) == 5: # 5 Card Charlie
            win_amount = 1
        else:
            try:
                key = ''
                for card in player_cards:
                    key = key + card + ","
                key = key[:-1]
                key = key + "|" + dealer_cards[0]

                bot_decision = decision_chart[key]
            except KeyError:
                key = ''
                for card in reversed(player_cards):
                    key = key + card + ","
                key = key[:-1]
                key = key + "|" + dealer_cards[0]

                bot_decision = decision_chart[key]

            match bot_decision:
                case 'H': # Hitting
                    #print("Hitting")
                    player_cards.extend(DECK.pop())
                case 'S': # Standing
                    #print("Standing")
                    win_amount = final_decision(player_cards, dealer_cards, DECK)
                case 'D': # Doubling
                    #print("Doubling")
                    player_cards.extend(DECK.pop())
                    if (get_hand_value(player_cards) > 21):
                        win_amount = -2
                    elif (get_hand_value(player_cards) == 21 and rules.INSTANT_PAYOUT):
                        win_amount = 2
                    else:
                        win_amount =  2 * final_decision(player_cards, dealer_cards, DECK)
                case 'X': # Splitting
                    # Not implemented yet
                    win_amount = 0
    return win_amount

def final_decision(player_cards, dealer_cards, DECK):
    win_amount = -1

    while (get_hand_value(dealer_cards) < 17):
        dealer_cards.extend(DECK.pop())

    #print("Dealer Hand" + str(dealer_cards))

    dealer_value = get_hand_value(dealer_cards)
    player_value = get_hand_value(player_cards)
    
    if (dealer_value == 22 and rules.STAND_22): # If casino has stand 22 rule (so fking stupid)
        win_amount = 0
        #print("Standing 22")
    elif (dealer_value > 21): # Dealer Busts
        win_amount = 1
        #print("Player Wins! dealer busts")
        if (player_value == 21): # Catch blackjack for casinos that don't payout instant
            win_amount = rules.BJ_PAYOUT
            #print("Blackjack!")
    elif (dealer_value == player_value): # Stand off
        win_amount = 0
        #print("Standing")
    elif (dealer_value < player_value): # Player Wins
        win_amount = 1
        #print("Player Wins!")
        if (player_value == 21): # Catch blackjack for casinos that don't payout instant
            win_amount = rules.BJ_PAYOUT
            #print("Blackjack!")
        #print("Dealer wins you lose!")
    # Otherwise player loses

    return win_amount
        