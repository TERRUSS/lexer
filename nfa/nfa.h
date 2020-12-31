
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	unsigned int to;
	char character;
} Transition;


typedef struct {
	unsigned int id;

	bool terminal;
	// state is initial if id==0

	Transition * transitions;
	unsigned int nTransitions;
} State;


typedef struct {
	State * states;
	unsigned int nStates;
} NFA;

	// Functions

void initEmptyLang(NFA * automaton);
void initEmptyWord(NFA * automaton);
void initChar(NFA * automaton, char chr);

void autUnion(NFA * automaton, NFA aut1, NFA aut2);
void autConcatenation(NFA * automaton, NFA aut1, NFA aut2);
void kleeneClosure(NFA * automaton);