CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TESTDIR = tests
SRCDIR = core

test_deck: test_deck.o deck.h
	$(CC) $(CFLAGS) test_deck.o -o test_deck

test_deck.o: test_deck.c
	$(CC) $(CFLAGS) -c test_deck.c -o test_deck.o

clean:
	rm -f *.o blackjack