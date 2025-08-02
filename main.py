import math
import numpy as np
from core.logic import *
from interface.file_io import *
from core.simulation import *
import json

# ==========================================================================================

# ==========================================================================================
if __name__ == "__main__":
    # Dictionary of the expected value for every starting hand and dealer card combination if decision is stand
    stand_EVs = {}
    double_EVs = {}
    hit_EVs = {}
    split_EVs = {}
    
    decision_chart = {}

    if (os.path.exists("data/decisions.json")):
        #print("Using existing data within data/")
        #print("Running simulation...")

        decision_chart = json_to_dict("data/decisions.json")

        run_simulation(decision_chart ,100000)
    else:
        print("Data does not exist!")
        print("Generating now...")
        analyse_blackjack(stand_EVs, double_EVs, hit_EVs, split_EVs, decision_chart)
    
        for name, _dict in [["standing", stand_EVs], ["doubling", double_EVs], ["hitting", hit_EVs], ["splitting", split_EVs], ["decisions", decision_chart]]:
            save_to_json(name, _dict)
            print("Successfully written to " + name + ".json")
        
        create_blackjack_db(hit_EVs, stand_EVs, double_EVs, split_EVs)
        print("Successfully written to blackjack_stats.db")

    #print("finished")