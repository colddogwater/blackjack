CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: blackjack

blackjack: driver.o blackjack.o
	$(CC) $(CFLAGS) -o blackjack driver.o blackjack.o

driver.o: driver.c blackjack.h
	$(CC) $(CFLAGS) -c driver.c

blackjack.o: blackjack.c blackjack.h
	$(CC) $(CFLAGS) -c blackjack.c

clean:
	rm -f *.o blackjack