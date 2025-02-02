# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Directories
SRCDIR = core
TESTDIR = tests
BUILDDIR = build
CLIDIR = cli

# Source files
SRCS = $(SRCDIR)/deck.c $(SRCDIR)/hand.c $(SRCDIR)/game.c $(CLIDIR)/cli_interface.c main.c
OBJS = $(SRCS:.c=.o)
TESTS = $(TESTDIR)/test_deck.c $(TESTDIR)/test_hand.c $(TESTDIR)/test_game.c

# Object files for tests
TEST_DECK_OBJ = $(TESTDIR)/test_deck.o $(SRCDIR)/deck.o
TEST_HAND_OBJ = $(TESTDIR)/test_hand.o $(SRCDIR)/hand.o $(SRCDIR)/deck.o
TEST_GAME_OBJ = $(TESTDIR)/test_game.o $(SRCDIR)/game.o $(SRCDIR)/hand.o $(SRCDIR)/deck.o

# Targets
.PHONY: all clean test_deck test_hand test_game

# Build entire program
all: blackjack

# Build blackjack program
blackjack: $(OBJS)
	$(CC) $(CFLAGS) $^ -o blackjack

# Build individual tests
test_deck: $(TEST_DECK_OBJ)
	$(CC) $(CFLAGS) $^ -o test_deck

test_hand: $(TEST_HAND_OBJ)
	$(CC) $(CFLAGS) $^ -o test_hand

test_game: $(TEST_GAME_OBJ)
	$(CC) $(CFLAGS) $^ -o test_game

# Compile test object files
$(TESTDIR)/%.o: $(TESTDIR)/%.c $(SRCDIR)/deck.h $(SRCDIR)/hand.h $(SRCDIR)/game.h $(CLIDIR)/cli_interface.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source object files
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/deck.h $(SRCDIR)/hand.h $(SRCDIR)/game.h $(CLIDIR)/cli_interface.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up all compiled files
clean:
	rm -rf $(SRCDIR)/*.o $(TESTDIR)/*.o blackjack test_deck test_hand test_game
