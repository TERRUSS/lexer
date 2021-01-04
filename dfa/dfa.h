#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../nfa/nfa.h"


//Deterministic finite automaton

typedef struct {
	unsigned int id;

	bool terminal;
	// State will be considered initial when id==0

	// Stores the transition for each possible character
	// -1 for no transition, destination state id for transition
	int transitions [256];

} DState;



typedef struct {
	DState * states;
	unsigned int nStates;
} DFA;



//Used to create states when converting NFA to DFA

typedef struct {
	//Array of non-deterministic states
	int * states;
	int nStates;

	int transitions [256];
} SingleState;



	//Functions

//Use to test if a word is recognized by DFA
bool testWord_DFA(DFA automaton, char * string);

//Converts NFA to DFA
DFA initDFAfromNFA(NFA automaton);
bool compareArrays(int len, int * arr1, int * arr2);

//Minimize a DFA
bool arePartitionsEquals(int ** partition, int ** partition2, int nLines);
bool arePartitionLinesEquals(int * line, int * line2);
DFA minimiseDFA(DFA automaton);
