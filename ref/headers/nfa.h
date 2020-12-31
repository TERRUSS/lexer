#pragma once

#include <stdbool.h>

//Nondeterministic automaton


    // Structs

typedef struct {
    unsigned int to; //Don't need 'from' since going to be stored inside States
    char character;
}Transition;


typedef struct {
    unsigned int id;

    bool terminal;	//By convention, state will be considered initial when id==0

    Transition * transitions;
    unsigned int nTransitions;

}State;


typedef struct {
    State * states;
    unsigned int nStates;
}NFAutomaton;



	// Functions

//Returns an automaton accepting the empty language
void initEmptyLang(NFAutomaton * automaton);

//Returns an automaton accepting  only the empty word
void initEmptyWord(NFAutomaton * automaton);

//Returns an automaton accepting  a character
void initChar(NFAutomaton * automaton, char chr);

//Returns union of automatons aut1|aut2
void autUnion(NFAutomaton * automaton, NFAutomaton aut1, NFAutomaton aut2);

//Returns concatenation of automatons aut1.aut2
void autConcatenation(NFAutomaton * automaton, NFAutomaton aut1, NFAutomaton aut2);

//Returns aut's iterative Kleene closure
void kleeneClosure(NFAutomaton * automaton);
