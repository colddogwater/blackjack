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

![Common BJ Rules](/imgs/normal.png)
Simulated balance results from most common blackjack rules using the optimal strategy table generated.

![Crown BJ Rules](/imgs/crown.png)
Simulated balance results from crown blackjack rules using the optimal strategy table generated. The modified rules allow the dealer to stand on 22 but also allows them to instantly pay out for blackjack. As you can see just this small change yields a negative expected value.