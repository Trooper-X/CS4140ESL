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
 * This is the linked list handler.
 * It pre-sorts the list [0-9] -> [a-z].
 * @param Links to the head of the linked list.
 * @param A pointer to the character array of the word.
 * @param This is the length of the character array of the word.
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
				printf("[%d] create newLine:\t{ %p, %p, %c, %d }\r\n", depth, 0, 0, theWord[depth], 0);
				wordList newLine = { 0, 0, theWord[depth], 0 };
				current->nextLine = &newLine;
			}
			// Insert before
			if (current->nextLine->theLetter > theWord[depth]){
				printf("[%d] insert newLine:\t{ %p, %p, %c, %d }\r\n", depth, current->nextLine, 0, theWord[depth], 0);
				wordList newLine = { current->nextLine, 0, theWord[depth], 0 };
				current->nextLine = &newLine;
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
					printf("[%d] insert newInLine:\t{ %p, %p, %c, %d }\r\n", depth, current->nextInLine, 0, theWord[depth], 0);
					wordList newInLine = { current->nextInLine, 0, theWord[depth], 0 };
					current->nextInLine = &newInLine;
				}
				// Else we should go to the next line, which is always done.
			}
			// There is no new in line, let's create one.
			else {
				printf("[%d] create newInLine:\t{ %p, %p, %c, %d }\r\n", depth, current->nextInLine, 0, theWord[depth], 0);
				wordList newInLine = { 0, 0, theWord[depth], 0 };
				current->nextInLine = &newInLine;
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
 * This is the main function.
 * No input arguments have been implemented.
 */
int main(int argc, char **argv) {
#if VERBOSE > 20
	printf("%s\r\n", __FUNCTION__);
#endif

#if 0
	wordList myWordList6 = { 0, 0, 'e', 1 };
	wordList myWordList5 = { 0, &myWordList6, 'j', 0 };
	wordList myWordList4 = { 0, 0, 's', 4 };
	wordList myWordList3 = { &myWordList4, &myWordList5, 'p', 1 };
	wordList myWordList2 = { 0, &myWordList3, 'a', 0 };
	wordList myWordList = { 0, &myWordList2, 'a', 2 };
#else
	wordList myWordList = {0,0,'0',0};
	listHandler(&myWordList, "i", 1);
	listHandler(&myWordList, "a", 1);
	listHandler(&myWordList, "aap", 3);
	listHandler(&myWordList, "aapje", 5);
	listHandler(&myWordList, "iphone", 6);
	listHandler(&myWordList, "aas", 3);
#endif

	// Print the output
//	printStructure(&myWordList, 0, 0);

	return 0;
}

/**
 * References
 * [1] https://en.wikipedia.org/wiki/Longest_word_in_English
 */
