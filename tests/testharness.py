import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from core.logic import *

print("Running tests...")

#If deck is 1 then impossible to have hands such as [A,2,2,2,2,2] as there is not enough 2's
player_hand = ['A', '2', '2', '2', '2']
dealer_card = ['7']

win, lose, push = hitting_tree(player_hand, dealer_card)

print(str(win) +","+ str(lose)+ ","+str(lose))