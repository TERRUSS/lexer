#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/nfa.h"


void initEmptyLang(NFAutomaton * automaton) {
	
    automaton->states = (State*) malloc(sizeof(State));
    automaton->states[0].id = 0;
    automaton->states[0].terminal = false;
    automaton->states[0].transitions = NULL;
    automaton->states[0].nTransitions = 0;

    automaton->nStates = 1;

}



void initEmptyWord(NFAutomaton * automaton) {

    automaton->states = (State*) malloc(sizeof(State));
    automaton->states[0].id = 0;
    automaton->states[0].terminal = true;
    automaton->states[0].transitions = NULL;
    automaton->states[0].nTransitions = 0;

    automaton->nStates = 1;
}



void initChar(NFAutomaton * automaton, char chr) {

	automaton->states = NULL;
	automaton->states = (State*) malloc(2 * sizeof(State));

	automaton->states[0].id = 0;
	automaton->states[0].terminal = false;

	automaton->states[0].transitions = NULL;
	automaton->states[0].transitions = malloc(sizeof(Transition));
	automaton->states[0].transitions[0].to = 1;
	automaton->states[0].transitions[0].character = chr;
	automaton->states[0].nTransitions = 1;

	automaton->states[1].id = 1;;
	automaton->states[1].terminal = true;

	automaton->states[1].transitions = false;
	automaton->states[1].nTransitions = 0;

	automaton->nStates = 2;

}



void autUnion(NFAutomaton * automaton, NFAutomaton aut1, NFAutomaton aut2) {

		//1) Adjust states IDs
		for(int i=0; i<aut2.nStates; i++) {
			aut2.states[i].id = aut2.states[i].id + aut1.nStates - 1;

			for(int j=0; j<aut2.states[i].nTransitions; j++) {
				//We assume the automaton is standard, so no transition to -1 (initial state)
				aut2.states[i].transitions[j].to = aut2.states[i].transitions[j].to + aut1.nStates - 1;
			}
		}

		//2) Set up new automaton with its initial state (union of both aut1's and aut2's initial states)
		automaton->nStates = aut1.nStates + aut2.nStates - 1;

		automaton->states = (State *) malloc(automaton->nStates * sizeof(State));

		automaton->states[0].id = 0;
		automaton->states[0].terminal = aut1.states[0].terminal || aut2.states[0].terminal;

		automaton->states[0].nTransitions = aut1.states[0].nTransitions + aut2.states[0].nTransitions;

		automaton->states[0].transitions = (Transition*) malloc(automaton->states[0].nTransitions * sizeof(Transition));
		memcpy(automaton->states[0].transitions, aut1.states[0].transitions, aut1.states[0].nTransitions * sizeof(Transition));
		memcpy(automaton->states[0].transitions + aut2.states[0].nTransitions * sizeof(Transition), aut2.states[0].transitions, aut2.states[0].nTransitions * sizeof(Transition));


		//3) Add other states

		//1st automaton
		for(int i=1; i<aut1.nStates; i++) {
			automaton->states[i].id = aut1.states[i].id;
			automaton->states[i].terminal = aut1.states[i].terminal;

			automaton->states[i].nTransitions = aut1.states[i].nTransitions;

			automaton->states[i].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));
			memcpy(automaton->states[i].transitions, aut1.states[i].transitions, automaton->states[i].nTransitions * sizeof(Transition));
		}

		//2nd automaton
		for(int i=1; i<aut2.nStates; i++) {
			automaton->states[aut1.nStates+i-1].id = aut2.states[i].id;
			automaton->states[aut1.nStates+i-1].terminal = aut2.states[i].terminal;

			automaton->states[aut1.nStates+i-1].nTransitions = aut2.states[i].nTransitions;

			automaton->states[aut1.nStates+i-1].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));

			memcpy(automaton->states[aut1.nStates+i-1].transitions, aut2.states[i].transitions, aut2.states[i].nTransitions * sizeof(Transition));

		}
}



void autConcatenation(NFAutomaton * automaton, NFAutomaton aut1, NFAutomaton aut2) {

	//1) Adjust states IDs

	for(int i=0; i<aut2.nStates; i++) {
		aut2.states[i].id = aut2.states[i].id + aut1.nStates - 1;

		for(int j=0; j<aut2.states[i].nTransitions; j++) {
			//We assume the automaton is standard, so no transition to -1 (initial state)
			aut2.states[i].transitions[j].to = aut2.states[i].transitions[j].to + aut1.nStates - 1;
		}
	}


	//2) Build final automaton by putting all states in it (except aut2[0])

	automaton->nStates = aut1.nStates + aut2.nStates - 1;

	automaton->states = (State*) malloc(automaton->nStates * sizeof(State));

	//First automaton
	for(int i=0; i<aut1.nStates; i++) {
		automaton->states[i].id = aut1.states[i].id;
		automaton->states[i].terminal = aut1.states[i].terminal;

		automaton->states[i].nTransitions = aut1.states[i].nTransitions;

		automaton->states[i].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));
		memcpy(automaton->states[i].transitions, aut1.states[i].transitions, automaton->states[i].nTransitions * sizeof(Transition));

		//3) Add transitions from aut1's terminal states to aut2's initial state's reachable states
		if(automaton->states[i].terminal) {

			automaton->states[i].terminal = aut2.states[0].terminal;

			int oldNTransitions = automaton->states[i].nTransitions;

			automaton->states[i].nTransitions += aut2.states[0].nTransitions;
			automaton->states[i].transitions = (Transition*) realloc(automaton->states[i].transitions, automaton->states[i].nTransitions * sizeof(Transition));

			for(int j=0; j<aut2.states[0].nTransitions; j++) {
				automaton->states[i].transitions[oldNTransitions + j].to = automaton->states[i].transitions[j].to;
				automaton->states[i].transitions[oldNTransitions + j].character = automaton->states[i].transitions[j].character;
			}
		}
	}


	//2nd automaton
	for(int i=1; i<aut2.nStates; i++) {
		automaton->states[aut1.nStates+i-1].id = aut2.states[i].id;
		automaton->states[aut1.nStates+i-1].terminal = aut2.states[i].terminal;

		automaton->states[aut1.nStates+i-1].nTransitions = aut2.states[i].nTransitions;

		automaton->states[aut1.nStates+i-1].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));

		memcpy(automaton->states[aut1.nStates+i-1].transitions, aut2.states[i].transitions, aut2.states[i].nTransitions * sizeof(Transition));

	}
}



void kleeneClosure(NFAutomaton * automaton) {
	//Assumes automaton is standard

	//1) Initial syate becomes terminal
	automaton->states[0].terminal = true;


	//2) Other terminal states are connected to states accesible from initial state through same char
	for(int i=1; i<automaton->nStates; i++) {
		if(automaton->states[i].terminal) {
			automaton->states[i].transitions = (Transition*) realloc(automaton->states[i].transitions, (automaton->states[i].nTransitions + automaton->states[0].nTransitions) * sizeof(Transition));
			memcpy(automaton->states[i].transitions + automaton->states[i].nTransitions * sizeof(Transition), automaton->states[0].transitions, automaton->states[0].nTransitions * sizeof(Transition));
			automaton->states[i].nTransitions += automaton->states[0].nTransitions;
		}
	}

}
