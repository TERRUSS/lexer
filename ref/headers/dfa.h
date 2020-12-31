#pragma once

#include "nfa.h"
#include <stdbool.h>
#include <string.h>

//Deterministic finite automaton


    // Structs

typedef struct {
    unsigned int id;

    bool terminal;
    //By convention, state will be considered initial when id==0
	//It implies that our automatons have at most 1 initial state

	//Stores the transition for each possible character
	//-1 for no transition, destination state id for transition
    int transitions [256];

}DState;



typedef struct {
    DState * states;
    unsigned int nStates;
}DFAutomaton;



//Used to create states when converting NFA to DFA

typedef struct {
	//Array of non-deterministic states
	int * states;
	int nStates;

	int transitions [256];
	//int * nTransitions [256];

}SingleState;



	//Functions

//Use to test if a word is recognized by DFA
bool testWord(DFAutomaton automaton, char * string);

//Converts NFA to DFA
DFAutomaton powersetConstruction(NFAutomaton automaton);
bool compareArrays(int len, int * arr1, int * arr2);

//Minimize a DFA
DFAutomaton brzozowski(DFAutomaton automaton);
void transpose(NFAutomaton * transposedAut, DFAutomaton aut);
