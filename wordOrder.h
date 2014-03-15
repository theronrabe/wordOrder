/*
Theron Rabe
wordOrder.h

	This file contains headers and type definitions used by the wordOrder program,
	found in wordOrder.c
*/
#ifndef _wordOrder_h_
#define _wordOrder_h_ 

typedef struct Letter {				//Represents a letter in an anagram linked-list
	char L;						//What letter?
	int count;					//How many?
	struct Letter *next;				//link to next node
} Letter;

typedef struct Anagram {			//Represents potential combinations of letters.
	Letter *head;					//linked-list
	int length;					//keep count stored, instead of recalculating all the time
} Anagram;

Letter *letterCreate(char L);				//Makes new Letter
Letter *letterDestroy(Letter *L);			//Returns pointer to next Letter

Anagram *anagramCreate();				//Builds a new anagram
unsigned long anagramOrder(char *word);			//Returns the order of the word in a sorted anagram list
int anagramInsertLetter(Anagram *A, char L);		//Adds a letter to the sorted anagram letter list
unsigned long anagramCombinations(Anagram *A);		//Returns the total possible combinations for the anagram
void anagramDestroy(Anagram *A);			//Frees allocated memory

unsigned long factorial(int X);				//Need this for calculating combinations
#endif
