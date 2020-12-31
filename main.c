#include "nfa/nfa.h"
#include "dfa/dfa.h"

#include "utils/printAutomatons.h"

int main(void) {

	//Testing NFA

	NFA aut1;
	aut1.states = NULL;
	aut1.nStates = 0;

	NFA aut2;
	aut2.states = NULL;
	aut2.nStates = 0;

	initChar(&aut1, 'a');
	printNFA("a", aut1);

	initChar(&aut2, 'b');
	printNFA("b", aut2);

	NFA aut3;
	aut3.states = NULL;
	aut3.nStates = 0;

	autConcatenation(&aut3, aut1, aut2);
	printNFA("a·b", aut3);

	NFA aut7;
	aut7.states = NULL;
	aut7.nStates = 0;
	autUnion(&aut7, aut1, aut2);
	printNFA("a|b", aut7);


	NFA aut4;
	aut4.states = NULL;
	aut4.nStates = 0;

	initChar(&aut4, 'c');
	printNFA("c", aut4);

	NFA aut5;
	aut5.states = NULL;
	aut5.nStates = 0;

	autConcatenation(&aut5, aut3, aut4);
	printNFA("(a·b)·c", aut5);

	NFA aut6;
	aut6.states = NULL;
	aut6.nStates = 0;

	autUnion(&aut6, aut5, aut4);
	printNFA("((a·b)·c)|c", aut6);


	kleeneClosure(&aut6);
	printNFA("(((a·b)·c)|c)*", aut6);


	//Testing DFA
	NFA automaton;

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
	automaton.states[1].id = 1;
	automaton.states[1].terminal = false;

	automaton.states[1].transitions = (Transition*) malloc(1 * sizeof(Transition));
	automaton.states[1].nTransitions = 1;

	automaton.states[1].transitions[0].to = 3;
	automaton.states[1].transitions[0].character = 'a';

	//State 2
	automaton.states[2].id = 2;
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

	printNFA("NFA dAutomaton", automaton);

	DFA dAutomaton = powersetConstruction(automaton);

	printDFA("DFA construction of dAutomaton", &dAutomaton);

	printf("Automaton %s %s the word %s \n\n", "dAutomaton", testWord_DFA(dAutomaton, "bc") ? "detects" : "does not detects", "bc");

	DFA minAut = minimiseDFA(dAutomaton);
	printDFA("minified DFA dAutomaton", &minAut);

    return 0;
}
