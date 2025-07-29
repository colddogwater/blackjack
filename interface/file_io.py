import json
import os

def save_to_json(name, _dict):
    os.makedirs("data", exist_ok=True) 
    filepath = os.path.join("data", name + ".json")
    with open(filepath, "w") as json_file:
        json.dump(_dict, json_file, indent=4)