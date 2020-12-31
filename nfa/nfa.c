
#include "nfa.h"
#include <stdio.h>


/*
 *	renvoie un automate standard reconnaissant le langage vide
 */
void initEmptyLang(NFA * automaton) {
	
	automaton->states = (State*) malloc(sizeof(State));
	automaton->states[0].id = 0;
	automaton->states[0].terminal = false;
	automaton->states[0].transitions = NULL;
	automaton->states[0].nTransitions = 0;

	automaton->nStates = 1;
}


/*
 *	renvoie un automate standard reconnaissant le langage composé du seul mot vide
 */
void initEmptyWord(NFA * automaton) {

	automaton->states = (State*) malloc(sizeof(State));
	automaton->states[0].id = 0;
	automaton->states[0].terminal = true;
	automaton->states[0].transitions = NULL;
	automaton->states[0].nTransitions = 0;

	automaton->nStates = 1;
}


/*
 *	renvoie  un  automate  standard  reconnaissant  le  langage  composéd’un mot d’un caractère passé en paramètre
 */
void initChar(NFA * automaton, char chr) {

	automaton->states = NULL;
	automaton->states = (State*) malloc(2 * sizeof(State));

	automaton->states[0].id = 0;
	automaton->states[0].terminal = false;

	automaton->states[0].transitions = NULL;
	automaton->states[0].transitions = malloc(sizeof(Transition));
	automaton->states[0].transitions[0].to = 1;
	automaton->states[0].transitions[0].character = chr;
	automaton->states[0].nTransitions = 1;

	automaton->states[1].id = 1;
	automaton->states[1].terminal = true;

	automaton->states[1].transitions = false;
	automaton->states[1].nTransitions = 0;

	automaton->nStates = 2;
}


/*
 *	prend deux automates standard en paramètre
 *	renvoie un automate standard reconnaissant la réunion de leurs langages
 */
void autUnion(NFA * automaton, NFA aut1, NFA aut2) {

	// Set up new automaton with its initial state
	automaton->nStates = aut1.nStates + aut2.nStates + 2;

	automaton->states = (State *) malloc(automaton->nStates * sizeof(State));

	automaton->states[0].id = 0;
	automaton->states[0].terminal = false;

	automaton->states[0].nTransitions = 2;
	automaton->states[0].transitions = (Transition*) malloc(automaton->states[0].nTransitions * sizeof(Transition));
	automaton->states[0].transitions[0].to = aut1.states[0].id + 1;
	automaton->states[0].transitions[0].character = 0;
	automaton->states[0].transitions[1].to = aut1.nStates +1;
	automaton->states[0].transitions[1].character = 0;

	// Adjust states IDs
	for(int i=0; i<aut1.nStates; i++) {
		aut1.states[i].id = aut1.states[i].id + 1;

		for(int j=0; j<aut1.states[i].nTransitions; j++) {
			// automate standard => pas de transitions vers l'état initial
			aut1.states[i].transitions[j].to = aut1.states[i].transitions[j].to + 1;
		}
	}
	aut1.states[aut1.nStates-1].nTransitions = aut1.states[aut1.nStates-1].nTransitions + 1;
	aut1.states[aut1.nStates-1].transitions = realloc(aut1.states[aut1.nStates-1].transitions, aut1.states[aut1.nStates-1].nTransitions * sizeof(Transition));
	aut1.states[aut1.nStates-1].transitions[aut1.states[aut1.nStates-1].nTransitions-1].to = automaton->nStates -1;
	aut1.states[aut1.nStates-1].transitions[aut1.states[aut1.nStates-1].nTransitions-1].character = 0;

	for(int i=0; i<aut2.nStates; i++) {
		aut2.states[i].id = aut2.states[i].id + aut1.nStates -1;

		for(int j=0; j<aut2.states[i].nTransitions; j++) {
			// automate standard => pas de transitions vers l'état initial
			aut2.states[i].transitions[j].to = aut2.states[i].transitions[j].to + aut1.nStates;
		}
	}
	aut2.states[aut2.nStates-1].nTransitions = aut2.states[aut2.nStates-1].nTransitions + 1;
	aut2.states[aut2.nStates-1].transitions = realloc(aut2.states[aut2.nStates-1].transitions, aut2.states[aut2.nStates-1].nTransitions * sizeof(Transition));
	aut2.states[aut2.nStates-1].transitions[aut2.states[aut2.nStates-1].nTransitions-1].to = automaton->nStates -1;
	aut2.states[aut2.nStates-1].transitions[aut2.states[aut2.nStates-1].nTransitions-1].character = 0;


	// Add other states

	//1st automaton
	for(int i=0; i<aut1.nStates; i++) {
		automaton->states[i+1].id = aut1.states[i].id;
		automaton->states[i+1].terminal = aut1.states[i].terminal;

		automaton->states[i+1].nTransitions = aut1.states[i].nTransitions;

		automaton->states[i+1].transitions = (Transition*) malloc(automaton->states[i+1].nTransitions * sizeof(Transition));
		memcpy(automaton->states[i+1].transitions, aut1.states[i].transitions, automaton->states[i+1].nTransitions * sizeof(Transition));
	}

	//2nd automaton
	for(int i=0; i<aut2.nStates; i++) {
		automaton->states[aut1.nStates+i+1].id = aut2.states[i].id;
		automaton->states[aut1.nStates+i+1].terminal = aut2.states[i].terminal;

		automaton->states[aut1.nStates+i+1].nTransitions = aut2.states[i].nTransitions;

		automaton->states[aut1.nStates+i+1].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));

		memcpy(automaton->states[aut1.nStates+i+1].transitions, aut2.states[i].transitions, aut2.states[i].nTransitions * sizeof(Transition));
	}

	// append the last terminal state
	automaton->states[automaton->nStates-1].id = automaton->nStates-1;
	automaton->states[automaton->nStates-1].terminal = true;
	automaton->states[automaton->nStates-1].transitions = NULL;
	automaton->states[automaton->nStates-1].nTransitions = 0;

}

/*
 *	prend un automate standard en paramètre
 *	renvoie un automate standard reconnaissant la concaténation de leurs langages
 */
void autConcatenation(NFA * automaton, NFA aut1, NFA aut2) {

	// Adjust states IDs

	for(int i=0; i<aut2.nStates; i++) {
		aut2.states[i].id = aut2.states[i].id + aut1.nStates - 1;

		for(int j=0; j<aut2.states[i].nTransitions; j++) {
			//We assume the automaton is standard, so no transition to -1 (initial state)
			aut2.states[i].transitions[j].to = aut2.states[i].transitions[j].to + aut1.nStates - 1;
		}
	}


	// Build final automaton by putting all states in it (except aut2[0])

	automaton->nStates = aut1.nStates + aut2.nStates - 1;

	automaton->states = (State*) malloc(automaton->nStates * sizeof(State));

	//First automaton
	for(int i=0; i<aut1.nStates; i++) {
		automaton->states[i].id = aut1.states[i].id;
		automaton->states[i].terminal = aut1.states[i].terminal;

		automaton->states[i].nTransitions = aut1.states[i].nTransitions;

		automaton->states[i].transitions = (Transition*) malloc(automaton->states[i].nTransitions * sizeof(Transition));
		memcpy(automaton->states[i].transitions, aut1.states[i].transitions, automaton->states[i].nTransitions * sizeof(Transition));

		// Add transitions from aut1's terminal states to aut2's initial state's reachable states
		if(automaton->states[i].terminal) {

			automaton->states[i].terminal = aut2.states[0].terminal;

			int oldNTransitions = automaton->states[i].nTransitions;

			automaton->states[i].nTransitions += aut2.states[0].nTransitions;
			automaton->states[i].transitions = (Transition*) realloc(automaton->states[i].transitions, automaton->states[i].nTransitions * sizeof(Transition));

			for(int j=0; j<aut2.states[0].nTransitions; j++) {
				automaton->states[i].transitions[oldNTransitions + j].to = aut2.states[0].transitions[j].to;
				automaton->states[i].transitions[oldNTransitions + j].character = aut2.states[0].transitions[j].character;
			}
		}
	}


	//2nd automaton
	for(int i=1; i<aut2.nStates; i++) {

		automaton->states[aut1.nStates+i-1].id = aut2.states[i].id;
		automaton->states[aut1.nStates+i-1].terminal = aut2.states[i].terminal;

		automaton->states[aut1.nStates+i-1].nTransitions = aut2.states[i].nTransitions;

		automaton->states[aut1.nStates+i-1].transitions = (Transition*) malloc(automaton->states[aut1.nStates+i-1].nTransitions * sizeof(Transition));

		memcpy(automaton->states[aut1.nStates+i-1].transitions, aut2.states[i].transitions, aut2.states[i].nTransitions * sizeof(Transition));

	}
}


/*
 *	renvoie un automate standard reconnaissant la fermeture itérative de Kleene de son langage
 */
void kleeneClosure(NFA * automaton) {
	// Initial state becomes terminal
	automaton->states[0].terminal = true;


	// Other terminal states are connected to states accesible from initial state through same char
	for(int i=1; i<automaton->nStates; i++) {
		if(automaton->states[i].terminal) {
			automaton->states[i].transitions = (Transition*) realloc(automaton->states[i].transitions, (automaton->states[i].nTransitions + automaton->states[0].nTransitions) * sizeof(Transition));
			memcpy(automaton->states[i].transitions + automaton->states[i].nTransitions * sizeof(Transition), automaton->states[0].transitions, automaton->states[0].nTransitions * sizeof(Transition));
			automaton->states[i].nTransitions += automaton->states[0].nTransitions;
		}
	}

}