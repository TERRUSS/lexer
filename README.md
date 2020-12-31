
# Projet de Compilation

## Automate fini non déterministe

### Présentation
Un Algorithme fini non déterministe (NFA : Nondeterministic Finite Algorithme) peut s'implémenter simplement à l'aide de quelques structures :

```c
typedef struct {
	unsigned int to;
	char character;
} Transition;


typedef struct {
	unsigned int id;

	bool terminal;

	Transition * transitions;
	unsigned int nTransitions;
} State;


typedef struct {
	State * states;
	unsigned int nStates;
} NFA;
```
Ainsi NFA n'est rien de plus d'un tableau d'états.
Chaque état indique s'il est initial (ie. s'il est d'ID 0 ou terminal, et une liste des transitions possible à partir de lui.
Une transition consiste en une référence vers le prochain état et l'étiquette de celle ci (ie. le caractère qu'il reconnait).

### Méthodes

#### Construction d’automate

	- initEmptyLang
```c
void initEmptyLang(NFA * automaton)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant le langage vide.
Pour ce qui est de l'implémentation, l'automate est composé que d'un état initial sans transitions.

	- initEmptyWord
```c
void initEmptyWord(NFA * automaton)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant le langage composé du seul mot vide.
Pour ce qui est de l'implémentation, l'automate est composé que d'un état initial et terminal sans transitions.

	- initChar
```c
void initChar(NFA * automaton, char chr)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant le langage composé d’un mot d’un caractère passé en paramètre.
L'automate est composé de deux états : l'état initial ayant une transition possible vers le second état (terminal) reconnaissant le caractère `chr`.

#### Méthodes avancées

	- autUnion
```c
void autUnion(NFA * automaton, NFA aut1, NFA aut2)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant la réunion de deux langages.
On suit la méthode des automates standard : l'état initial du nouvel automate est l'union des états initiaux des deux automates en entrée, et on ajoute ensuite les autres états des automates d'entrée.

	- autConcatenation
```c
void autConcatenation(NFA * automaton)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant la concaténation de deux langages.
En suivant la méthode des automates standard : on ajoute les transitions de l'état terminal du premier automate d'entrée vers l'état initial du second.

	- kleeneClosure
```c
void kleeneClosure(NFA * automaton)
```
`[INSERT SCREENSHOT OF THE PRINT_AUTOMATA FUNCTION]`
Permet de créer un un automate standard reconnaissant la fermeture itérative de Kleene de son langage.
On suit la méthode des automates standard : on rend terminal l'état initial de l'automate, puis on le rend accessible par les états terminaux via le même caractère de sa transition.

## Automate fini déterministe