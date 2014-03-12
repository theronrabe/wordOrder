/*
Theron Rabe
wordOrder.c

	This program takes a word as input, and outputs the index in which that word would appear
	in a sorted list of its anagrams. This program completes in linear complexity, and does
	not produce an exhaustive list of input anagrams.

	This is accomplished according to the following algorithm:

		1. Do an insertion sort of input characters into linked list (an "Anagram" structure)
		2. For each character of input word, left to right
			2a. output += (index_of_character_in_list * number_of_anagram_combinations) / number_of_characters_in_list
			2b. remove character from list
		3. Print accumulated output

*/
#include "wordOrder.h"
#include <stdio.h>
#include <stdlib.h>

/*
	main:	program entry point. Turns input word into an Anagram,
		then measures the order of the input word within that Anagram.
*/
int main(int argc, char **argv) {
	Anagram *A;			//space for an Anagram structure
	unsigned long order;		//a return value

	if (argc > 1) {
		A = anagramCreate(argv[1]);				//turn input into Anagram structure
		order = anagramOrder(A, argv[1]);			//measure order of input within anagram

		printf("Order of word %s: %ld.\n\n", argv[1], order);	//print results
		anagramDestroy(A);
	} else {
		printf("Oops! You forgot to supply a word.\n\n");
	}
	return 0;
}


/*
	anagramOrder:	determines the relative position of a word within a
			sorted set of anagrams.
*/
unsigned long anagramOrder(Anagram *A, char *word) {
	Letter *next;			//a Letter iterator
	int i = 0;			//a character iterator
	int length, index;		//temp values for calculations
	unsigned long combos;
	unsigned long accumulator = 1;	//an accumulated result

	if (A) {
		while (*(word+i)) {				//for each character in word
			length = A->length;				//grab anagram statistics
			combos = anagramCombinations(A);
			index = anagramTakeLetter(A, *(word+i));

			if (combos < 2) return accumulator;		//break early, if combinations are low enough

			accumulator += (index * combos)/length;		//increase accumulator according to statistics
			++i;
		}
		return accumulator;
	}

	return -1;
}


/*
	factorial:	returns the factorial of an input.
			Needed to compute total anagram combinations.
*/
unsigned long factorial(int X) {
	int i;				//an iterator
	unsigned long accumulator = 1;	//an accumalted result
	unsigned long oldAcc = accumulator;	//for overflow checking
	
	for (i=2; i<=X; i++) {		//for each X,
		oldAcc = accumulator;
		accumulator *= i;		//multiply accumulator by iterator
		if (accumulator < oldAcc) {
			printf("Error. Input too long.\n\n");
			exit(1);
		}
	}

	return accumulator;
}


/*
	letterCreate:	turns an input character into a Letter node for an Anagram.
*/
Letter *letterCreate(char L) {
	Letter *new = (Letter *) malloc(sizeof(Letter));	//allocate memory
	new->L = L;						//set default values
	new->count = 1;
	new->next = NULL;
	return new;
}

/*
	letterDestroy:	frees memory associated to Letter node.
*/
Letter *letterDestroy(Letter *L) {
	Letter *next = L->next;		//grab return value before destroyed
	free(L);			//free memory
	return next;			//return next letter
}

/*
	anagramCreate:	turns an input word into its correlating Anagram structure.
*/
Anagram *anagramCreate(char *word) {
	Anagram *new = (Anagram *) malloc(sizeof(Anagram));	//allocate memory
	new->head = NULL;					//set default values
	new->length = 0;
	int i = 0;

	while (*(word+i)) {
		anagramInsertLetter(new, *(word+i));		//insert letters from word
		++i;
	}

	return new;
}

/*
	anagramInsertLetter:	contributes a character to an Anagram structure.
*/
void anagramInsertLetter(Anagram *A, char L) {
	Letter *newLetter;			//space for new letter
	Letter *next, *prev;			//iterators

	if (A) {
		A->length++;			//increment length of anagram
		prev = NULL;
		next = A->head;

		while (next) {					//for each iteration...
			if (next->L == L) {				//if L found, increment count. return.
				next->count++;					
				return;

			} else if (next->L > L) {			//if L is a new letter, insert into anagram. return.
				newLetter = letterCreate(L);
				newLetter->next = next;
				if (prev) {prev->next = newLetter;} else {A->head = newLetter;}
				return;
			}

			prev = next;
			next = next->next;
		}

		newLetter = letterCreate(L);					//Create L at end of list
		if (prev) {prev->next = newLetter;} else {A->head = newLetter;}
	}
}

/*
	anagramTakeLetter:	returns the index of specified character, and removes it from the anagram.
*/
int anagramTakeLetter(Anagram *A, char L) {
	int index = 0;				//a return value
	Letter *next, *prev;			//iterators

	if (A) {
		prev = NULL;
		next = A->head;

		while (next) {					//for each iteration...
			if (next->L == L) {				//if L located, reduce counts
				next->count--;
				A->length--;

				if (next->count < 1) {			//adjust list (if needed)
					if (prev) {
						prev->next = next->next;
					} else {
						A->head = next->next;
					}
					letterDestroy(next);
				}
				return index;				//return index
			}
			index += next->count;			//iterate and try again until located
			prev = next;
			next = next->next;
		}
		return -1;
	}
}

/*
	anagramCombinations:	returns the number of possible combinations the anagram could be in.
*/
unsigned long anagramCombinations(Anagram *A) {
	unsigned long multiplicity = 1;					//degree of letter redundancy
	unsigned long oldMul = multiplicity;				//for overflow checking
	Letter *next;							//an iterator

	if (A) {
		next = A->head;
		while (next) {
			oldMul = multiplicity;
			multiplicity *= factorial(next->count);		//accumulate multiplicity
			if (multiplicity < oldMul) {
				printf("Error. Input too large.\n\n");
				exit(1);
			}
			next = next->next;
		}
		return factorial(A->length)/multiplicity;		//calculate and return total combinations
	}
	return 0;
}

/*
	anagramDestroy:		frees memory associated to an anagram structure.
*/
void anagramDestroy(Anagram *A) {
	Letter *next;						//an iterator

	if (A) {
		next = A->head;
		while (next) {next = letterDestroy(next);}	//free letter iterations
		free(A);					//free anagram
	}
}
