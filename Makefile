
CC = gcc

O_FILES = nfa.o dfa.o
FLAGS = -Wall -Werror -g

all: build clean

build: $(O_FILES)
	$(CC) main.c $(O_FILES) $(FLAGS)

nfa.o: nfa/nfa.c
	$(CC) -c $< $(FLAGS) -o $@

dfa.o: dfa/dfa.c
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm *.o
