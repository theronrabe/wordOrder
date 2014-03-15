/*
Theron Rabe
wordOrder.c

	This program takes a word as input, and outputs the index in which that word would appear
	in a sorted list of its anagrams. This program does not produce an exhaustive list of input anagrams.

	This is accomplished according to the following algorithm:

		1. Iteratively insert each letter of input (right to left) into sorted Anagram structure
		2. For each iteration...
			output += index_of_character_in_list * number_of_anagram_combinations / number_of_characters_in_list

		3. Print accumulated output

*/
#include "wordOrder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	main:	program entry point.
		measures order of input within its sorted anagrams
		then prints the result
*/
int main(int argc, char **argv) {
	unsigned long order;						//a return value

	if (argc > 1) {
		order = anagramOrder(argv[1]);				//measure order of input within anagrams

		printf("Order of word %s:\t%lu.\n\n", argv[1], order);	//print results
	} else {
		printf("Oops! You forgot to supply a word.\n\n");
	}
	return 0;
}


/*
	anagramOrder:	determines the relative position of a word within a
			sorted set of anagrams.
*/
unsigned long anagramOrder(char *word) {
	Anagram *A = anagramCreate();				//make a new anagram
	int i = strlen(word) - 1;				//a character iterator
	unsigned long order = 1;				//default return value
	int index, length;					//temporary storage
	unsigned long combos;
	unsigned long add;

	while (*(word+i)) {					//for each character in word, from right to left...
		index = anagramInsertLetter(A, *(word+i));		//insert character into Anagram, finding its index
		combos = anagramCombinations(A);			//grab possible Anagram combinations
		length = A->length;					//and length

		add = index * (combos/length);				//find amount to increment return value by
		add += (combos%length)? add * (combos%length) / length : 0;		//increase increment amount, if fraction is uneven

		order += add;						//increase return value
		--i;
	}

	anagramDestroy(A);					//clean up

	return order;						//return
}

/*
	anagramInsertLetter:	contributes a character to an Anagram structure.
				returns the index at which the character was inserted.
*/
int anagramInsertLetter(Anagram *A, char L) {
	Letter *newLetter;			//space for new letter
	Letter *next, *prev;			//iterators
	int index = 0;				//a return value

	if (A) {
		A->length++;			//increment length of anagram
		prev = NULL;
		next = A->head;

		while (next) {					//for each iteration...
			if (next->L == L) {				//if L found, increment count. return.
				next->count++;					
				return index;

			} else if (next->L > L) {			//if L is a new letter, insert into anagram. return.
				newLetter = letterCreate(L);
				newLetter->next = next;
				if (prev) {prev->next = newLetter;} else {A->head = newLetter;}
				return index;
			}

			index += next->count;				//increase index of new letter

			prev = next;
			next = next->next;				//reiterate
		}

		newLetter = letterCreate(L);			//Create L at end of list
		if (prev) {prev->next = newLetter;} else {A->head = newLetter;}
		
		return A->length - 1;
	}
	return -1;
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
	anagramCreate:	makes a new anagram structure and sets default values
*/
Anagram *anagramCreate() {
	Anagram *A = (Anagram *) malloc(sizeof(Anagram));	//allocate memory for anagram
	A->head = NULL;						//set default values
	A->length = 0;

	return A;
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

