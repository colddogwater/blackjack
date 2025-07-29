from core import rules

# GLOBAL VARIABLES
RANKS = ['2', '3', '4', '5', '6', '7', '8', '9', 'J', 'A']
NO_RANKS= ['4', '4', '4', '4', '4', '4', '4', '4', '16', '4']
RANK_TO_VALUE = {'2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, 'J': 10, 'A': 11}
DECK = 4 * RANKS

# Main logic for iterating through all hands and calculating
def analyse_blackjack(stand_EVs, double_EVs, hit_EVs, split_EVs, decision_chart):
    # Loop through all possible combinations
    # Works for hitting, doubling, and standing
    for first_card in RANKS:
        for second_card in RANKS:
            player_hand = [first_card, second_card]
            player_key = first_card+","+ second_card
            for dealer_card in RANKS:
                key = player_key+"|"+dealer_card
                
                if (get_hand_value(player_hand) == 21):
                    if (rules.INSTANT_PAYOUT):
                        stand_EVs[key] = rules.BJ_PAYOUT
                    else:
                        stand_EVs[key] = get_expected_value(standing_tree, player_hand,[dealer_card], rules.BJ_PAYOUT, 1)
                else:
                    stand_EVs[key] = get_expected_value(standing_tree, player_hand,[dealer_card], 1, 1)
                    double_EVs[key] = get_expected_value(double_tree, player_hand,[dealer_card], 2, 2)
                    hit_EVs[key] = get_expected_value(hitting_tree, player_hand,[dealer_card], 1, 1)
    
    # Splitting requires all other dictionaries to already be populated
    # Thus has to be another loop :(
    for card in RANKS:
        player_hand = [card, card]
        player_key = card+","+ card
        
        for dealer_card in RANKS:
            key = player_key+"|"+dealer_card
            split_EVs[key] = splitting_tree(player_hand, dealer_card, stand_EVs, double_EVs, hit_EVs)
    
    # Finally, with all dictionaries populated we can look through every hand and see which dictionary has the
    # highest EV, then that should be the decision the player makes for that hand
    for first_card in RANKS:
        for second_card in RANKS:
            player_hand = [first_card, second_card]
            player_key = first_card+","+ second_card
            for dealer_card in RANKS:
                key = player_key+"|"+dealer_card

                # Create decision dictionary
                ev_standing = stand_EVs.get(key, -float('inf'))
                ev_doubling = double_EVs.get(key, -float('inf'))
                ev_hitting = hit_EVs.get(key, -float('inf'))
                ev_splitting = split_EVs.get(key, -float('inf'))

                # Determine the maximum expected value and the corresponding action
                max_ev = max(ev_standing, ev_doubling, ev_hitting, ev_splitting)

                if max_ev == ev_standing:
                    decision_chart[key] = 'S'
                elif max_ev == ev_doubling:
                    decision_chart[key] = 'D'
                elif max_ev == ev_splitting:
                    decision_chart[key] = 'X'
                else: 
                    decision_chart[key] = 'H'

#--------------------- STANDING
# Evaluates the probabilities of winning, losing and pushing given that the player is standing
def standing_tree(player_cards, dealer_cards):
    prob_win = 0
    prob_lose = 0
    prob_push = 0

    player_total = get_hand_value(player_cards)
    all_cards = player_cards + dealer_cards
    #Loop through potential cards that the dealer could draw
    #For each card determine its likelihood of being drawn and what the outcome of the game would be
    for i, potential_card in enumerate(RANKS):
        potential_card_value = RANK_TO_VALUE[potential_card]
        total_dealer_value = get_hand_value(dealer_cards) + RANK_TO_VALUE[potential_card]
        
        # Number of cards in the deck take away how many we can see in player or dealer hand
        probability_of_card = get_card_prob(all_cards, potential_card, i)

        if (potential_card_value == 11 and total_dealer_value > 21):
            total_dealer_value -= 10
        
        if (total_dealer_value == 22 and rules.STAND_22):
            prob_push += probability_of_card
        elif (total_dealer_value > 21):
            prob_win += probability_of_card
        elif (total_dealer_value > 16):
            if total_dealer_value > player_total:
                prob_lose += probability_of_card
            elif total_dealer_value < player_total:
                prob_win += probability_of_card
            else:
                prob_push += probability_of_card
        else:
            #Recursively adds card to dealer hand and then evaluates the odds of this new hand
            d_card_temp = dealer_cards + [potential_card]

            win_Recurs, lose_Recurs, push_Recurs = standing_tree(player_cards, d_card_temp)
            prob_win += win_Recurs * probability_of_card
            prob_lose += lose_Recurs * probability_of_card
            prob_push += push_Recurs * probability_of_card

    return prob_win, prob_lose, prob_push

#--------------------- DOUBLING
# Evaluates the probabilities of winning, pushing and losing given the player has doubled
# For each card the player recieves after the double, calculate the standing tree probabilities for each
def double_tree(player_cards, dealer_cards):
    prob_win = 0
    prob_lose = 0
    prob_push = 0

    player_total = get_hand_value(player_cards)
    all_cards = player_cards + dealer_cards
    #Loop through all potential cards that the player could recieve 
    #For each card determine its likelihood of being drawn and what the outcome of the game would be
    for i, potential_card in enumerate(RANKS):
        possible_hand = player_cards + [potential_card]

        probability_of_card = get_card_prob(all_cards, potential_card, i)
        
        if (get_hand_value(possible_hand) > 21):
            prob_lose += probability_of_card
        elif (get_hand_value(possible_hand) == 21 and rules.INSTANT_PAYOUT):
            prob_win += probability_of_card
        else: 
            prob_win_tc, prob_lose_tc, prob_push_tc = standing_tree(possible_hand,dealer_cards)
    
            prob_win += prob_win_tc * probability_of_card
            prob_lose += prob_lose_tc * probability_of_card
            prob_push += prob_push_tc * probability_of_card
            
    return prob_win, prob_lose, prob_push

#--------------------- HITTING
# Evaluates the probabilities of winning, pushing and losing given the player has chosen to hit
# For each card the player recieves after the hit, calculate the standing tree probabilities for each and the hitting probabilities.
# If the probability of winning is larger for hitting tree then use those probabilities otherwise use standing. 
# This seems like a collapse of information here, I am going to rework it in the future so that the dictionaries contain all possible
# hitting odds aswell so that down the line you can analyze any possible situation you are in
# I.e. If I have [6][6] and dealer shows [10] maybe hitting is best option, so you hit and recieve a 2
# Question is now  what is the new best decision for the following state P=[6][6]][2], D=[10]?
def hitting_tree(player_cards, dealer_cards):
    prob_win = 0
    prob_lose = 0 
    prob_push = 0

    player_total = get_hand_value(player_cards)
    all_cards = player_cards + dealer_cards
    for i, potential_card in enumerate(RANKS):
        possible_hand = player_cards + [potential_card]

        probability_of_card = get_card_prob(all_cards, potential_card, i)
        
        if (get_hand_value(possible_hand) > 21):
            prob_lose += probability_of_card
        elif (len(possible_hand) == 5): # 5 Card Charlie
            prob_win += probability_of_card
        else:
            prob_win_stand, prob_lose_stand, prob_push_stand = standing_tree(possible_hand, dealer_cards)
            prob_win_hit, prob_lose_hit, prob_push_hit = hitting_tree(possible_hand, dealer_cards)

            EV_stand = prob_win_stand - prob_lose_stand
            EV_hit = prob_win_hit - prob_lose_hit

            # check whether its more likely to stand or hit
            if (EV_hit > EV_stand):
                prob_win += prob_win_hit * probability_of_card
                prob_lose += prob_lose_hit * probability_of_card
                prob_push += prob_push_hit * probability_of_card    
            else:
                prob_win += prob_win_stand * probability_of_card
                prob_lose += prob_lose_stand * probability_of_card
                prob_push += prob_push_stand * probability_of_card
            
    return prob_win, prob_lose, prob_push

#--------------------- SPLITTING
# Current implementation is really an approximationg
# - Only allows splitting once
# - Does not track all the cards for probability_of_card calculation
# - 2 * ev will always be an approximation
def splitting_tree(player_cards, dealer_cards, standing_e_values, double_e_values, hitting_e_values):
    ev = 0

    player_total = get_hand_value(player_cards)
    all_cards = player_cards + [dealer_cards]

    split_hand = [player_cards[0]]
    for i, potential_card in enumerate(RANKS):
        possible_hand = split_hand + [potential_card]
        all_cards = all_cards + [potential_card]

        probability_of_card = get_card_prob(all_cards, potential_card, i)
        #print(str(possible_hand) + "| with dealer " + potential_card )
        if (get_hand_value(possible_hand) == 21):
            ev += rules.BJ_PAYOUT * probability_of_card
        else:
            
            #Janky Code, because of how I have structured the for loops it skips over symmetric pairs
            # I.e. [2,3] is the same as  [3,2] thus ignore [3,2] and save computation
            # But when using as a key it is now causing trouble
            try:
                key = possible_hand[0]+","+ possible_hand[1]+"|"+dealer_cards[0]
                EV_hit = hitting_e_values[key]
                EV_stand = standing_e_values[key]
                EV_double = double_e_values[key]
            except KeyError:
                key = possible_hand[1]+","+ possible_hand[0]+"|"+dealer_cards[0]
                EV_hit = hitting_e_values[key]
                EV_stand = standing_e_values[key]
                EV_double = double_e_values[key]
            #print("Taking EV as " + str(max(EV_hit, EV_double, EV_stand)))

            ev += max(EV_hit, EV_double, EV_stand) * probability_of_card

    #Approximate splitting by doubling the EV
    return 2 * ev

#--------------------- HELPER FUNCTIONS

# Returns the rank total of any size hand including Ace logic
def get_hand_value(cards):
    value = 0
    num_aces = 0
    for card in cards:
        if card == 'A':
            num_aces += 1
        value += RANK_TO_VALUE[card]

    # Handle Aces as 1 or 11
    while value > 21 and num_aces > 0:
        value -= 10
        num_aces -= 1
    return value

def get_card_prob(all_cards, card, i):
    return (int(NO_RANKS[i]) * rules.NO_DECKS - (all_cards.count(card))) / (rules.NO_DECKS * 52 - len(all_cards))

def get_expected_value(tree_function, player_cards, dealer_cards, win_constant, lose_constant):
    prob_win, prob_lose, prob_push = tree_function(player_cards, dealer_cards)
    return (win_constant * prob_win) - (lose_constant * prob_lose)