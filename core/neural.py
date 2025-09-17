import numpy as np
from core import rules
from core.logic import get_best_choice, get_hand_value, RANK_TO_VALUE
import random

ACTIONS = {
    0: 'H',  # Hit
    1: 'S',  # Stand
    2: 'D',  # Double
    3: 'X'   # Split
}

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

def encode_state(player_cards, dealer_cards):
    total = get_hand_value(player_cards)
    usable_ace = int('A' in player_cards and total <= 11 + 10)
    dealer_up = dealer_cards[0]  # just the visible card
    dealer_val = RANK_TO_VALUE[dealer_up]
    return np.array([total, usable_ace, dealer_val], dtype=np.float32)


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

class BlackjackEnvWrapper:
    def __init__(self):
        self.deck = []
        self.discard_pile = []
        self.reset_deck()

    def reset_deck(self):
        self.deck = 4 * ['2','3','4','5','6','7','8','9','J','J','J','J','A'] * rules.NO_DECKS
        random.shuffle(self.deck)
        self.discard_pile.clear()

    def reset(self):
        # deal fresh hand
        self.player = Hand([self.deck.pop(), self.deck.pop()])
        self.dealer = Hand([self.deck.pop()])
        return encode_state(self.player.cards, self.dealer.cards)

    def step(self, action_idx):
        action = ACTIONS[action_idx]
        reward = 0
        done = False

        if action == 'H':
            self.player.cards.append(self.deck.pop())
            total = get_hand_value(self.player.cards)
            if total > 21:
                done = True
                reward = -1

        elif action == 'S':
            # let dealer play out
            while get_hand_value(self.dealer.cards) < 17:
                self.dealer.cards.append(self.deck.pop())
            done = True
            reward = final_decision(self.player, self.dealer.cards, self.deck)

        elif action == 'D':
            self.player.cards.append(self.deck.pop())
            self.player.win_amount = 2
            if get_hand_value(self.player.cards) > 21:
                done = True
                reward = -2
            else:
                # dealer plays
                while get_hand_value(self.dealer.cards) < 17:
                    self.dealer.cards.append(self.deck.pop())
                done = True
                reward = final_decision(self.player, self.dealer.cards, self.deck)

        elif action == 'X':
            # Splits are trickier, you’d need to handle multiple hands
            # You can leave this out until the basic agent works
            pass

        state = encode_state(self.player.cards, self.dealer.cards)
        return state, reward, done, {}
