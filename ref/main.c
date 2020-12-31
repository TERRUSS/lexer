#include "headers/nfa.h"
#include "headers/dfa.h"

#include <stdio.h>
#include <stdlib.h>


int main(void) {

	//Testing NFA

	NFAutomaton aut1;
	aut1.states = NULL;
	aut1.nStates = 0;

	NFAutomaton aut2;
	aut2.states = NULL;
	aut2.nStates = 0;

	initChar(&aut1, 'a');
	initChar(&aut2, 'b');

	NFAutomaton aut3;
	aut3.states = NULL;
	aut3.nStates = 0;

	autConcatenation(&aut3, aut1, aut2);

	NFAutomaton aut4;
	aut4.states = NULL;
	aut4.nStates = 0;

	initChar(&aut4, 'c');

	NFAutomaton aut5;
	aut5.states = NULL;
	aut5.nStates = 0;

	autConcatenation(&aut5, aut3, aut4);

	NFAutomaton aut6;
	aut6.states = NULL;
	aut6.nStates = 0;

	autUnion(&aut6, aut5, aut4);

	kleeneClosure(&aut6);



	//Testing DFA

	NFAutomaton automaton;

	automaton.states = (State*) malloc(4 * sizeof(State));
	automaton.nStates = 4;

	//State 0
	automaton.states[0].id = 0;
	automaton.states[0].terminal = false;

	automaton.states[0].transitions = (Transition*) malloc(3 * sizeof(Transition));
	automaton.states[0].nTransitions = 3;

	automaton.states[0].transitions[0].to = 1;
	automaton.states[0].transitions[0].character = 'a';

	automaton.states[0].transitions[1].to = 2;
	automaton.states[0].transitions[1].character = 'a';

	automaton.states[0].transitions[2].to = 2;
	automaton.states[0].transitions[2].character = 'b';

	//State 1
	automaton.states[1].id = 0;
	automaton.states[1].terminal = false;

	automaton.states[1].transitions = (Transition*) malloc(1 * sizeof(Transition));
	automaton.states[1].nTransitions = 1;

	automaton.states[1].transitions[0].to = 3;
	automaton.states[1].transitions[0].character = 'a';

	//State 2
	automaton.states[2].id = 0;
	automaton.states[2].terminal = false;

	automaton.states[2].transitions = (Transition*) malloc(1 * sizeof(Transition));
	automaton.states[2].nTransitions = 1;

	automaton.states[2].transitions[0].to = 3;
	automaton.states[2].transitions[0].character = 'c';

	//State 3
	automaton.states[3].id = 3;
	automaton.states[3].terminal = true;

	automaton.states[3].transitions = NULL;
	automaton.states[3].nTransitions = 0;


	DFAutomaton dAutomaton = powersetConstruction(automaton);

	testWord(dAutomaton, "ab");

	DFAutomaton minAut = brzozowski(dAutomaton);


    return 0;
}
