import math
import numpy as np
from core.logic import *
from interface.file_io import *

# ==========================================================================================

# ==========================================================================================
if __name__ == "__main__":
    # Dictionary of the expected value for every starting hand and dealer card combination if decision is stand
    stand_EVs = {}
    double_EVs = {}
    hit_EVs = {}
    split_EVs = {}
    
    decision_chart = {}

    analyse_blackjack(stand_EVs, double_EVs, hit_EVs, split_EVs, decision_chart)
    
    for name, _dict in [["standing", stand_EVs], ["doubling", double_EVs], ["hitting", hit_EVs], ["splitting", split_EVs], ["decisions", decision_chart]]:
        save_to_json(name, _dict)
        print("Successfully written to " + name + ".json")
    
    print("finished")