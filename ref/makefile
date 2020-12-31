FILES = nfa.o dfa.o
FLAGS = -Wall -Werror -g

all: build clean

build: $(FILES)
	gcc main.c $(FILES) $(FLAG)

nfa.o: headers/nfa.h
	gcc -c nfa.c $(FLAGS)

dfa.o: headers/dfa.h
	gcc -c dfa.c $(FLAGS)

clean:
	rm *.o
