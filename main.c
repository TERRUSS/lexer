#include "nfa/nfa.h"
#include "dfa/dfa.h"

#include "utils/printAutomatons.h"

int main(int argc, char ** argv) {

	// NFA

	NFA a;

	initChar(&a, 'a');
	printNFA("a", a);

	NFA b;
	initChar(&b, 'b');
	printNFA("b", b);

	NFA a_dot_b;
	autConcatenation(&a_dot_b, a, b);
	printNFA("a·b", a_dot_b);

	NFA a_or_b;
	autUnion(&a_or_b, a, b);
	printNFA("a|b", a_or_b);


	NFA c;
	initChar(&c, 'c');
	printNFA("c", c);

	NFA a_dot_b_dot_c;
	autConcatenation(&a_dot_b_dot_c, a_dot_b, c);
	printNFA("(a·b)·c", a_dot_b_dot_c);

	NFA a_dot_b_dot_c_or_C;
	autUnion(&a_dot_b_dot_c_or_C, a_dot_b_dot_c, c);
	printNFA("((a·b)·c)|c", a_dot_b_dot_c_or_C);


	kleeneClosure(&a_dot_b_dot_c_or_C);
	printNFA("(((a·b)·c)|c)*", a_dot_b_dot_c_or_C);



	// DFA

	// hardcoding a non-deterministic automaton wich can be converted to a deterministic one

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

	printNFA("test automaton", automaton);

	DFA dAutomaton = initDFAfromNFA(automaton);

	printDFA("d automaton generated", &dAutomaton);

	printf("The automaton \"%s\" %s the word : %s\n", "dAutomaton", testWord_DFA(dAutomaton, "bc") ? "detects" : "does not detect", "bc");
	printf("The automaton \"%s\" %s the word : %s \n\n", "dAutomaton", testWord_DFA(dAutomaton, "bcd") ? "detects" : "does not detect", "bcd");

	DFA minAut = minimiseDFA(dAutomaton);
	printDFA("minified DFA dAutomaton", &minAut);

	return EXIT_SUCCESS;
}
