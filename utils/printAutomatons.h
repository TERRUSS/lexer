
#include <stdio.h>

void printNFA(const char * title, NFA automaton){

	printf("===== %s (NFA) =====\n", title);
	int i,j;
	for(i=0; i<automaton.nStates; i++) {
		printf("\tState %d :\n", i);

		for(j=0; j<automaton.states[i].nTransitions; j++) {
				printf("\t\t--(%c)-> %d\n", automaton.states[i].transitions[j].character, automaton.states[i].transitions[j].to);
			}

			if(automaton.states[i].terminal) {
				printf("\t\t--x \n");
			}
		}
		printf("\n\n");

}

void printDFA(const char * title, DFA* automaton){

	printf("===== %s (DFA) =====\n", title);
	int i, character;
	for(i=0; i<automaton->nStates; i++) {
		printf("\tState %d :\n", automaton->states[i].id);

		for(character=0; character<256; character++) {

				int from = automaton->states[i].transitions[character];

				if(from == -1) {
					continue;
				}

				printf("\t\t--(%c)-> %d\n", character, from);
			}

			if(automaton->states[i].terminal) {
				printf("\t\t--x \n");
			}
		}
		printf("\n\n");

}
