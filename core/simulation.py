import random
from core import rules
from core.logic import get_hand_value
    
class Hand:
    ALIVE = 0
    BUSTED = -1
    WIN = 1
    DOUBLE = 2
    STANDING = 3

    def __init__(self, cards=None, status=0, win_amount=1):
        self.cards = cards if cards is not None else []
        self.status = status
        self.win_amount = win_amount

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

        #player_cards = ['6','6']
        #dealer_cards = ['5']

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
        if len(discard_pile) > (0.1 * 52 * rules.NO_DECKS):
            #print("Reshuffling")
            DECK.extend(discard_pile)
            random.shuffle(DECK)
            discard_pile.clear()

        player_cards = []
        dealer_cards = []
 
def game_loop(player_cards, dealer_cards, DECK, decision_chart):
    win_amount = 0
    #Create player_hands array to hold all future potential splits
    initial_hand = Hand(player_cards)
    dealer_hand = Hand(dealer_cards)

    #player_hands = [None] * 6
    player_hands = [initial_hand]
    
    # Updates hand for
    # Populates player_hands (if player splits) and also determines the end state of every hand
    player_turn(player_hands, dealer_hand, DECK, decision_chart)
    #player_hands will be populated with hands
    # each hand will have a gamestate, alive or bust, doubled or blackjack win etc etc
   
    if(player_hands[0].status == Hand.BUSTED):
        #print("Player busted, skipping dealer drawing")
        win_amount = player_hands[0].win_amount * -1
    else:
        # Draw dealer cards
        while (get_hand_value(dealer_hand.cards) < 17):
            dealer_hand.cards.extend(DECK.pop())

        #print("Dealer Hits to: " + str(dealer_hand.cards))

        for hand in player_hands:
            #print("---- Hand")
            win_amount += final_decision(hand, dealer_hand.cards, DECK)

    return win_amount



def player_turn(player_hands, dealer_hand, DECK, decision_chart, split_depth=0):
    curr_hand = player_hands[split_depth]

    while curr_hand.status == Hand.ALIVE:
        #print("Player cards: " + str(player_cards))
        hand_value = get_hand_value(curr_hand.cards)
        if hand_value == 21 and rules.INSTANT_PAYOUT:
            curr_hand.status = Hand.WIN
            # Only pays BJ Ratio if its 2 cards
            if len(curr_hand.cards) == 2:
                curr_hand.win_amount = rules.BJ_PAYOUT
        elif hand_value > 21:
            curr_hand.status = Hand.BUSTED
        elif len(curr_hand.cards) == 5: # 5 Card Charlie
            curr_hand.status = Hand.WIN
        else:
            #How the dictionary was populated removes duplicates
            # i.e. dict['A,J'] may exist but dict['J,A'] does not
            try:
                key = ''
                for card in curr_hand.cards:
                    key = key + card + ","
                key = key[:-1]
                key = key + "|" + dealer_hand.cards[0]

                bot_decision = decision_chart[key]
            except KeyError:
                key = ''
                for card in reversed(curr_hand.cards):
                    key = key + card + ","
                key = key[:-1]
                key = key + "|" + dealer_hand.cards[0]

                bot_decision = decision_chart[key]

            match bot_decision:
                case 'H': # Hitting
                    #print("Hitting")
                    curr_hand.cards.extend(DECK.pop())
                    #print("New player hand: " + str(curr_hand.cards))
                case 'S': # Standing
                    #print("Standing")
                    curr_hand.status = Hand.STANDING
                    #win_amount = final_decision(player_cards, dealer_cards, DECK)
                case 'D': # Doubling
                    #print("Doubling")
                    curr_hand.cards.extend(DECK.pop())
                    curr_hand.win_amount = 2
                    #print("New player hand: " + str(curr_hand.cards))
                    if (get_hand_value(curr_hand.cards) > 21):
                        curr_hand.status = Hand.BUSTED
                    elif (get_hand_value(curr_hand.cards) == 21 and rules.INSTANT_PAYOUT):
                        curr_hand.status = Hand.WIN
                    else:
                        curr_hand.status = Hand.STANDING
                case 'X': # Splitting
                    if (curr_hand.cards == ['A','A']):
                        split_hand = Hand([curr_hand.cards.pop()])
                        split_hand.cards.extend(DECK.pop())
                        curr_hand.cards.extend(DECK.pop())

                        curr_hand.status = Hand.STANDING
                        split_hand.status = Hand.STANDING
                        player_hands.extend([split_hand])
                    else:
                        split_hand = Hand([curr_hand.cards.pop()])
                        split_hand.cards.extend(DECK.pop())
                        curr_hand.cards.extend(DECK.pop())
                        #print("Successfully split into " + str(curr_hand.cards) + " and " +str(split_hand.cards))

                        split_index = split_depth + 1
                        player_hands.extend([split_hand])
                        player_turn(player_hands, dealer_hand, DECK, decision_chart, split_index)
                    

    return 0

def final_decision(player_hand, dealer_cards, DECK):
    win_amount = -9999

    if (player_hand.status == Hand.BUSTED):
        #print("Player lose: From bust")
        win_amount = player_hand.win_amount * -1
    elif (player_hand.status == Hand.WIN):
        #print("Player wins: Instant win")
        win_amount = player_hand.win_amount
    else:
        dealer_value = get_hand_value(dealer_cards)
        player_value = get_hand_value(player_hand.cards)
        
        if (dealer_value == 22 and rules.STAND_22): # If casino has stand 22 rule (so fking stupid)
            win_amount = 0
            #print("No winner: Standing 22")
        elif (dealer_value > 21): # Dealer Busts
            win_amount = player_hand.win_amount
            #print("Player wins: dealer busts")
        elif (dealer_value == player_value): # Stand off
            win_amount = 0
            #print("No winner: Standing")
        elif (dealer_value < player_value): # Player Wins
            #print("Player wins: Dealer has less than player")
            win_amount = player_hand.win_amount
        else:
            #print("Player loses: Dealer has higher than player")
            win_amount = player_hand.win_amount * -1
        
    # Otherwise player loses

    return win_amount
        