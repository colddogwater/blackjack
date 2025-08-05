import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from core.logic import *
from core.simulation import *
from interface.file_io import *


DECK = ['A', 'A','4','A','A']

print(get_hand_value(['A','A']))

decision_chart = json_to_dict("../data/decisions.json")
player_cards = []
dealer_cards = []

print("---------------------------------")
player_cards.extend(DECK.pop())
player_cards.extend(DECK.pop())

dealer_cards.extend(DECK.pop())

print("Player hand: " + str(player_cards) + "Dealer hand: " + str(dealer_cards))
#print("Cards in deck: " + str(len(DECK)))

win_amount = game_loop(player_cards, dealer_cards, DECK, decision_chart)

balance += win_amount * wager 
print("Balance, " + str(balance))

