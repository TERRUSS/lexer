#include "headers/dfa.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//Deterministic finite automaton


bool testWord(DFAutomaton automaton, char * string) {
	unsigned int currentState = 0;


	for(int i=0; i<strlen(string); i++) {

		if(automaton.states[currentState].transitions[(int)string[i]] == -1) {
			return false;
		}

		currentState = automaton.states[currentState].transitions[(int)string[i]] ;
	}


	return automaton.states[currentState].terminal;
}



DFAutomaton powersetConstruction(NFAutomaton automaton) {

	//0) Initialization

	//Used to store new single states
 	SingleState * singleStates = NULL;

	singleStates = (SingleState*) malloc(sizeof(SingleState));
	for(int i=0; i<256; i++) {
		singleStates[0].transitions[i] = -1;
	}
	singleStates[0].states = (int*) malloc(sizeof(int));
	singleStates[0].states[0] = 0;
	singleStates[0].nStates = 1;


	int i = 0;
	int nStates = 1;

	//1) While there is still a new state to explore
	while(i < nStates) {

		//1.2) Check transitions on all chars
		int * accessibleStates = NULL;
		int nAccessibleStates = 0;



		for(int j=0; j<256; j++) {

			//1.2.1) RESET ACCESSIBLE STATES
			if(accessibleStates != NULL && nAccessibleStates != 0) {
				free(accessibleStates);
				accessibleStates = NULL;
				nAccessibleStates = 0;
			}


			//1.2.2) LOOK FOR ALL ACCESSIBLE STATES ON THIS CHAR

			//Check all original states included in new state
			for(int k=0; k<singleStates[i].nStates; k++) {

				//Check all transitions of the current original state
				for(int l=0; l<automaton.states[singleStates[i].states[k]].nTransitions; l++) {

					if( (int) automaton.states[singleStates[i].states[k]].transitions[l].character == j ) {
						nAccessibleStates++;
						accessibleStates = (int*) realloc(accessibleStates, nAccessibleStates * sizeof(int));
						accessibleStates[nAccessibleStates-1] = automaton.states[singleStates[i].states[k]].transitions[l].to;
					}
				}
			}

			if(nAccessibleStates == 0) {
				continue;
			}


			//1.2.3) CHECK IF IT IS A NEW STATE
			bool newState = true;
			int k;
			for(k=0; k<nStates; k++) {
				if(singleStates[k].nStates == nAccessibleStates) {

					//If state doesn't already exist
					if(compareArrays(nAccessibleStates, accessibleStates, singleStates[k].states)) {
						newState = false;
						break;
					}
				}
			}


			//1.2.4) IF NEWSTATE, ADD
			if(newState) {
				//Add state to singleStates
				nStates++;
				singleStates = realloc(singleStates, nStates*sizeof(SingleState));

				singleStates[nStates-1].states = (int*) malloc(nAccessibleStates * sizeof(int));

				for(int l=0; l<nAccessibleStates; l++) {
					singleStates[nStates-1].states[l] = accessibleStates[l];
				}

				singleStates[nStates-1].nStates = nAccessibleStates;

				for(int l=0; l<256; l++) {
					singleStates[nStates-1].transitions[l] = -1;
				}

				//Set transition to new state
				singleStates[i].transitions[j] = nStates-1;


			//1.2.5) ELSE, JUST ADD TRANSITION TO OLD STATE
			} else {
				//Set transition to old state (k)
				singleStates[i].transitions[j] = k;
			}
		}

		//Go to next state
		i++;
	}


	//2) Create DFA
	DFAutomaton dAutomaton;
	dAutomaton.states = (DState*)  malloc(nStates * sizeof(DState));
	dAutomaton.nStates = nStates;

	for(int i=0; i<nStates; i++) {
		dAutomaton.states[i].id = i;

		bool terminal = false;
		for(int j=0; j<singleStates[i].nStates; j++) {
			if(automaton.states[singleStates[i].states[j]].terminal) {
				terminal = true;
				break;
			}
		}

		dAutomaton.states[i].terminal = terminal;
		for(int j=0; j<256; j++) {
			dAutomaton.states[i].transitions[j] = singleStates[i].transitions[j];
		}
	}

	return dAutomaton;
}



bool compareArrays(int len, int * arr1, int * arr2) {
	for(int i=0; i<len; i++) {
		if(arr1[i] != arr2[i]) {
			return false;
		}
	}

	return true;
}



DFAutomaton brzozowski(DFAutomaton automaton) {

	//A~
	NFAutomaton nFAutomaton;
	nFAutomaton.states = NULL;
	nFAutomaton.nStates = 0;
	transpose(&nFAutomaton, automaton);
	DFAutomaton ATilde = powersetConstruction(nFAutomaton);


	//(A~)~ (= minimal automaton)
	NFAutomaton nFAutomaton2;
	nFAutomaton2.states = NULL;
	nFAutomaton2.nStates = 0;
	transpose(&nFAutomaton2, ATilde);
	DFAutomaton minAutomaton = powersetConstruction(nFAutomaton2);


	//Free
	for(int i=0; i<nFAutomaton.nStates; i++) {
			free(nFAutomaton.states[i].transitions);
	}

	for(int i=0; i<nFAutomaton2.nStates; i++) {
			free(nFAutomaton2.states[i].transitions);
	}


	return minAutomaton;
}



void transpose(NFAutomaton * transposedAut, DFAutomaton aut) {

	//1) Init transposedAut
	transposedAut->nStates = aut.nStates;
	transposedAut->states = (State*) malloc(transposedAut->nStates * sizeof(State));

	for(int i=0; i<transposedAut->nStates; i++) {
		transposedAut->states[i].id = i;
		transposedAut->states[i].terminal = false;

		transposedAut->states[i].transitions = NULL;
		transposedAut->states[i].nTransitions = 0;
	}


	//2) Initial/terminal states

	//Since our convention is to have the state of rank 0 to be initial, we
	//can only have one initial state. Since initial and terminal states are
	//exchanges, it implies we can only have one terminal state.
	//For this reason, the terminal and initial states will be pre-treated here
	//before continuing with the next step.
	//(We assume we have EXACTLY one terminal state)

	//Look for terminal state
	int terminalState;
	for(int i=1; i<transposedAut->nStates; i++) {
		if(aut.states[i].terminal) {
			terminalState = i;
			break;
		}
	}

	//Set up state 0 to be terminal in transposedAut
	transposedAut->states[0].terminal = true;

	//Adjust IDs in transitions
	for(int i=0; i<aut.nStates; i++) {
		for(int j=0; j<256; j++) {

			if(aut.states[i].transitions[j] == 0) {
				aut.states[i].transitions[j] = terminalState;
			}else if(aut.states[i].transitions[j] == terminalState) {
				aut.states[i].transitions[j] = 0;
			}
		}
	}


	//3) Build inverted transitions
	for(int i=0; i<aut.nStates; i++) {
		int to = i;
		if(i == 0) {
			to = terminalState;
		}

		for(int j=0; j<256; j++) {
			int from = aut.states[i].transitions[j];

			if(from == -1) {
				continue;
			}


			transposedAut->states[from].nTransitions++;
			transposedAut->states[from].transitions = (Transition*) realloc(transposedAut->states[from].transitions, transposedAut->states[from].nTransitions * sizeof(Transition));
			transposedAut->states[from].transitions[transposedAut->states[from].nTransitions-1].to = to;
			transposedAut->states[from].transitions[transposedAut->states[from].nTransitions-1].character = (char) j;
		}
	}


}
