
#include "dfa.h"


bool testWord_DFA(DFA automaton, char * string) {
	unsigned int currentState = 0;


	for(int i=0; i<strlen(string); i++) {

		if(automaton.states[currentState].transitions[(int)string[i]] == -1) {
			return false;
		}

		currentState = automaton.states[currentState].transitions[(int)string[i]] ;
	}


	return automaton.states[currentState].terminal;
}



DFA initDFAfromNFA(NFA automaton) {

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
	DFA dAutomaton;
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


DFA minimiseDFA(DFA automaton) {
		// using Moore's algorithm

	// partition declaration
	int ** partition, ** oldPartition;
	partition = malloc( automaton.nStates );
	oldPartition = malloc( automaton.nStates );
	for (int i = 0; i < automaton.nStates; i++){
		partition[i] = malloc( (256 + 1) * sizeof(int) );
		oldPartition[i] = malloc( (256 + 1) * sizeof(int) );
	}


	// init fist partition
	for (int i = 0; i < automaton.nStates; i++){
		partition[0][i] = automaton.states[i].terminal;
	}
	for (int i = 0; i<automaton.nStates; i++){
		for (int character = 0; character < 256 + 1; character++){
			if (automaton.states[i].transitions[character] != -1)
				partition[i][character+1] = automaton.states[automaton.states[i].transitions[character]].terminal;
		}
	}

	// main loop
	do {
		int count = 0;
		for (int i = 0; i < automaton.nStates; i++){
			memcpy(oldPartition[i], partition[i], (256+1) * sizeof(int));
		}

		for (int partitionLine = 0; partitionLine < automaton.nStates; ++partitionLine) {
			bool match = false;

			for (int i = partitionLine; i >= 0 && match == false; i--){
				if ( arePartitionLinesEquals(oldPartition[partitionLine], oldPartition[partitionLine-i]) ){
					partition[0][partitionLine] = partitionLine-i;
					match = true;
				}
			}
			if (match == false){
				partition[0][partitionLine] = count++;
			}
		}

		for (int i = 0; i<automaton.nStates; i++){
			for (int character = 0; character < 256 + 1; character++){
				if (automaton.states[i].transitions[character] != -1)
					partition[i][character+1] = partition[i][automaton.states[i].transitions[character]+1];
			}
		}

	} while ( !arePartitionsEquals(partition, oldPartition, automaton.nStates) );

	for (int partitionLine = 0; partitionLine < automaton.nStates; ++partitionLine) {
		bool match = false;

		for (int i = partitionLine; i >= 0 && match == false; i--){
			if (arePartitionLinesEquals(oldPartition[partitionLine], oldPartition[partitionLine-i])){
				partition[0][partitionLine] = partitionLine-i;
				match = true;
				//printf("%d = %d\n", partitionLine, partitionLine-i);
			}
		}
	}


	return automaton;
}


bool arePartitionsEquals(int ** partition, int ** partition2, int nLines){
	for (int i = 0; i<nLines; i++){
		if ( !arePartitionLinesEquals(partition[i], partition2[i]) ){
			return false;
		}
	}
	return true;
}

bool arePartitionLinesEquals(int * line, int * line2){
	return compareArrays(256+1, line, line2);
}
