/**
 * This project is for the CS4140 project entry.
 *
 * Author: 			Sander van Leeuwen
 * Student number:	4351436
 * Study:			Computer Engineering
 */

// Load some standard functions and types.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines:
#define VERBOSE 0

/**
 * Defines how the linked list should look like.
 */
typedef struct _wordList {
	struct _wordList *nextInLine; // This is if the character is not in this line
	struct _wordList *nextLine;	// This is if the character equals the one currently in line.
	char theLetter;	// Could be compressed to 36 unique characters (62 if capitals are considered).
	uint16_t count;	// Assuming words do not occur more than 65536 times
} wordList;

/**
 * Create a new element of the list structure.
 * Count is always initialized to zero.
 * @param nextInLine
 * @param nextLine
 * @param theLetter
 * @return A pointer to the newly created element.
 */
wordList *createElement(wordList *nextInLine, wordList *nextLine, char theLetter){
	wordList* newElement = malloc(sizeof(wordList));
	newElement->nextInLine = nextInLine;
	newElement->nextLine = nextLine;
	newElement->theLetter = theLetter;
	newElement->count = 0;
	return newElement;
}

/**
 * This is the linked list handler.
 * It pre-sorts the list [0-9] -> [a-z].
 * @param Links to the head of the linked list.
 * @param A pointer to the character array of the word.
 * @param This is the length of the character array of the word. Words longer than 255 characters are not expected [1].
 */
void listHandler(wordList *head, char *theWord, uint8_t wordLength) {
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif
	wordList *current = head;
	uint8_t depth = 0;

	// If
	while (depth <= wordLength) {
		// Check current character
		if (current->theLetter == theWord[depth]) {
			depth++;
			// Check if we are done and add to count
			if (depth >= wordLength) {
				current->count++;
				return;
			}
			// Create on missing line
			if (!current->nextLine) {
				current->nextLine = createElement(0, 0, theWord[depth]);
			}
			// Insert before
			if (current->nextLine->theLetter > theWord[depth]){
				current->nextLine = createElement(current->nextLine, 0, theWord[depth]);
			}
			// Jump to next line
			current = current->nextLine;
		}
		// Check for next in line
		else {
			// Check if there is a next line
			if (current->nextInLine) {
				// Check if the next character of the line is bigger so that structure in between can be made.
				if (current->nextInLine->theLetter > theWord[depth]) {
					// Insert in between
					current->nextInLine = createElement(current->nextInLine, 0, theWord[depth]);
				}
				// Else we should go to the next line, which is always done.
			}
			// There is no new in line, let's create one.
			else {
				current->nextInLine = createElement(0, 0, theWord[depth]);
			}
			// Make the current position the next in line
			current = current->nextInLine;
		}
	}
	printf("An error occurred.\r\n");
}

/**
 * [DEBUG]
 * Print the list structure on console.
 * @param The to be printed list.
 */
void printListStructure(wordList *lst) {
	printf("================================\r\n");
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif
	printf("Current:\t%p\r\n", lst);
	printf("nextInLine:\t%p\r\n", lst->nextInLine);
	printf("nextLine:\t%p\r\n", lst->nextLine);
	printf("theLetter:\t%c\r\n", lst->theLetter);
	printf("count:\t\t%d\r\n", lst->count);
}

/**
 * Print the structure ready for output.
 * @note This structure is called recursively!
 * @param The to be printed linked list.
 * @param To build words, this parameter points to the string of letters of the previous lines.
 * @param This parameter denotes the depth/ amount of letters to be printed.
 */
void printStructure(wordList *lst, char* previousLetters,
		uint8_t previousLettersCount) {
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif
	// If there is a count, it means this is a word!
	if (lst->count) {
#if VERBOSE > 5
		printListStructure(lst);
#endif
		for (uint8_t i = 0; i < previousLettersCount; i++) {
			printf("%c", previousLetters[i]);
		}
		printf("%c: %d\r\n", lst->theLetter, lst->count);
	}

	// Prepare the to be passed character string.
	char currentLetters[previousLettersCount + 1];
	for (uint8_t i = 0; i < previousLettersCount; i++)
		currentLetters[i] = previousLetters[i];
	currentLetters[previousLettersCount] = lst->theLetter;

	// Recursively print the next line
	if (lst->nextLine)
		printStructure(lst->nextLine, currentLetters, previousLettersCount + 1);

	// Recursively print the next in this line
	if (lst->nextInLine)
		printStructure(lst->nextInLine, previousLetters, previousLettersCount);
	// The last position of previousLettersCount is not being readout.
}

/**
 * Get the input and get the workers their share of the work.
 * @param The list to add the results to.
 */
void scanRoutine(wordList *lst){
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif
	int c;
	char word[255];
	uint8_t length = 0;
	// Read per character
	while((c = getchar()) != EOF){
#if VERBOSE > 40
		printf("[%c]\t%d\r\n", c, c);
#endif
		// Add to word if it is a character
		if( ((c >= 48) && (c <= 57)) || ((c >= 65) && (c <= 90)) || ((c >= 97) && (c <= 122)) ){
			word[length] = (char) c;
			length++;
		}
		// End of word
		else{
			// If there is a word in the buffer, add it.
			if(length){
#if VERBOSE > 40
				printf(">\t");
				for(uint8_t i = 0; i<length; i++)
					printf("%c",word[i]);
				printf("\r\n");
#endif
				listHandler(lst, word, length);
				length = 0;
			}
			// Else do nothing
		}
	}
#if VERBOSE >= 1
	printf("EOF\r\n");
#endif
}

/**
 * Free memory of the list
 * @param The list
 */
void freeList(wordList *lst){
	// Step to next if it exists
	if(lst->nextInLine)
		freeList(lst->nextInLine);
	if(lst->nextLine)
		freeList(lst->nextLine);

	// Free the current node
	free(lst);
}

/**
 * This is the main function.
 * No input arguments have been implemented.
 */
int main(int argc, char **argv) {
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif
	// Create root
	wordList *myWordList = createElement(0,0,'0');

	// Read the input
	scanRoutine(myWordList);

	// Print the output
	printStructure(myWordList, 0, 0);

	// Free the memory :D
	freeList(myWordList);

	return 0;
}

/**
 * References
 * [1] https://en.wikipedia.org/wiki/Longest_word_in_English
 */
