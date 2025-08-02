import json
import os
import sqlite3
import re

def save_to_json(name, _dict):
    os.makedirs("data", exist_ok=True) 
    filepath = os.path.join("data", name + ".json")
    with open(filepath, "w") as json_file:
        json.dump(_dict, json_file, indent=4)

def json_to_dict(json_path="decisions.json"):
    abs_path = os.path.abspath(json_path)
    try:
        with open(abs_path, 'r') as f:
            decision_chart = json.load(f)
        return decision_chart
    except FileNotFoundError:
        print(f"Error: File '{json_path}' not found.")
        return {}
    except json.JSONDecodeError:
        print(f"Error: File '{json_path}' is not valid JSON.")
        return {}

def create_blackjack_db(hit_EVs, stand_EVs, double_EVs, split_EVs, db_filename="blackjack_stats.db"):

    def calculate_hand_value(cards_str):
        """Calculate the numeric value of a hand, handling aces appropriately."""
        cards = cards_str.split(',')
        total = 0
        aces = 0
        
        for card in cards:
            card = card.strip()
            if card == 'J': 
                total += 10
            elif card == 'A':
                aces += 1
                total += 11
            else:
                total += int(card)
        
        while total > 21 and aces > 0:
            total -= 10
            aces -= 1
            
        return total
    
    def is_soft_hand(cards_str):
        cards = cards_str.split(',')
        total = 0
        aces = 0
        
        for card in cards:
            card = card.strip()
            if card == 'J':  
                total += 10
            elif card == 'A':
                aces += 1
                total += 11
            else:
                total += int(card)
        
        return aces > 0 and total <= 21
    
    def is_pair(cards_str):
        cards = cards_str.split(',')
        if len(cards) != 2:
            return False
        
        card1 = cards[0].strip()
        card2 = cards[1].strip()
        
        return card1 == card2
    
    data_dir = "data"
    os.makedirs(data_dir, exist_ok=True)
    
    db_path = os.path.join(data_dir, db_filename)
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS blackjack_stats (
            hand_value INTEGER,
            player_cards TEXT,
            dealer_card TEXT,
            action TEXT,
            expected_value REAL,
            issoft BOOLEAN,
            ispair BOOLEAN
        )
    ''')
    
    cursor.execute('DELETE FROM blackjack_stats')
    
    all_keys = set()
    all_keys.update(hit_EVs.keys())
    all_keys.update(stand_EVs.keys())
    all_keys.update(double_EVs.keys())
    all_keys.update(split_EVs.keys())
    
    records_inserted = 0
    
    for key in all_keys:
        try:
            player_cards, dealer_card = key.split('|')
            
            hand_value = calculate_hand_value(player_cards)
            issoft = is_soft_hand(player_cards)
            ispair = is_pair(player_cards)
            
            actions_data = [
                ('hit', hit_EVs.get(key)),
                ('stand', stand_EVs.get(key)),
                ('double', double_EVs.get(key)),
                ('split', split_EVs.get(key))
            ]
            
            for action, ev_value in actions_data:
                if ev_value is not None:  # Only insert if we have data for this action
                    cursor.execute('''
                        INSERT INTO blackjack_stats 
                        (hand_value, player_cards, dealer_card, action, expected_value, issoft, ispair)
                        VALUES (?, ?, ?, ?, ?, ?, ?)
                    ''', (hand_value, player_cards, dealer_card, action, ev_value, issoft, ispair))
                    records_inserted += 1
        
        except ValueError:
            print(f"Warning: Skipping malformed key: {key}")
            continue
    
    # Commit changes and close connection
    conn.commit()
    conn.close()
    
    return f"Database '{db_path}' created successfully with {records_inserted} records."

