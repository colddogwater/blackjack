# Blackjack Strategy Calculator

A Python application that re-produces the blackjack strategy tables seen online. Two HTML pages; the first visualises expected values for every initial player hand and dealer show card, and the other takes the largest expected value for each combination representing the best decision in every initial game state to maximise returns, yielding the previously mentioned optimal blackjack strategy tables which are common online. The `rules.py` allows slight differences in house rules unique to each casino to be implemented and accurately update the expected values and optimal decisions. A SQL database is created compiling all information into one place where the user can easily query, including EVs beyond the 2 card starting hands. Finally, this decision dictionary is used by a simulation to calculate the simulated EV.

## Features

- Produces EV and optimal strategy tables in HTML
- Changeable rule file to reflect differing house rules
- Export to `.db` and `.json`
- Blackjack simulation to determine simulated EV

## Project Structure

- `main.py`: Entry point
- `core/logic.py`: Core game logic and strategy code
- `core/simulation.py`: Simple code to simulate a game of blackjack randomly
- `interface/`: Handles DB and file operations
- `data/`: Output files

## Getting Started

```bash
pip install -r requirements.txt
python main.py
```

## Results
Snippet showcasing the expected value tables generated and visualised by the HTML files.
![EV Website](/imgs/evs.png)
Another snippet showcasing the strategy tables indicating the optimal decision for every starting hand.
![Strat Website](/imgs/strat.png)
All this data is stored into the `blackjack_stats.db` which can easily be opened in SQL and queried to gain more insight.
![SQL Data](/imgs/sql_showcase.png)

## Simulations
### Normal Blackjack House Rules
![Common BJ Rules](/imgs/6deck_normal.png)
Simulated balance results from most common blackjack rules using the optimal strategy table generated. 

### Crown Casino Perth Blackjack House Rules
![Crown BJ Rules](/imgs/6deck_CrownRules.png)
Only changed:
- Dealer stands on 22
- Reaching 21 instantly pays out the player

Even with two small rule changes, even one of them favouring the player, the house edge increases by an order of magnitude!